

#include "zebu.h"

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

const struct zz_list zz_list_empty = { NULL, NULL };

static struct zz_node *zz_node_alloc(struct zz_tree *tree)
{
    struct zz_node_chunk *chunk;
    struct zz_node *node;

    if (tree->node_chunk_size == ZZ_CHUNK_SIZE) {
        chunk = calloc(1, sizeof(*chunk));
        chunk->next = tree->nodes;
        tree->nodes = chunk;
        tree->node_chunk_size = 0;
    }
    node = &tree->nodes->data[tree->node_chunk_size];
    ++tree->node_chunk_size;
    node->tree = tree;
    return node;
}

static const char *zz_string_alloc(struct zz_tree *tree, const char *str)
{
    struct zz_string_chunk *chunk;
    int i;
    size_t len;
    char *buf;

    chunk = tree->strings;
    if (chunk != NULL) {
        for (i = 0; i < tree->string_chunk_size; ++i) {
            if (strcmp(chunk->data[i], str) == 0)
                return chunk->data[i];
        }
    }
    for (chunk = tree->strings; chunk; chunk = chunk->next) {
        for (i = 0; i < ZZ_CHUNK_SIZE; ++i) {
            if (strcmp(chunk->data[i], str) == 0)
                return chunk->data[i];
        }
    }
    if (tree->string_chunk_size == ZZ_CHUNK_SIZE) {
        chunk = calloc(1, sizeof(*chunk));
        chunk->next = tree->strings;
        tree->strings = chunk;
        tree->string_chunk_size = 0;
    }
    len = strlen(str) + 1;
    buf = malloc(strlen(str));
    memcpy(buf, str, len);
    tree->strings->data[tree->string_chunk_size] = buf;
    ++tree->string_chunk_size;
    return buf;
}

void zz_tree_init(struct zz_tree *tree,
        const struct zz_node_type * token_types, size_t token_types_size)
{
    tree->token_types = token_types;
    tree->token_types_size = token_types_size;
    tree->nodes = NULL;
    tree->node_chunk_size = ZZ_CHUNK_SIZE;
    tree->strings = NULL;
    tree->string_chunk_size = ZZ_CHUNK_SIZE;
}

void zz_tree_deinit(struct zz_tree * tree)
{
    int i;

    while (tree->nodes) {
        struct zz_node_chunk *next = tree->nodes->next;
        free(tree->nodes);
        tree->nodes = next;
    }
    if (tree->strings) {
        struct zz_string_chunk *next = tree->strings->next;
        for (i = 0; i < tree->string_chunk_size; ++i)
            free(tree->strings->data[i]);
        free(tree->strings);
        tree->strings = next;
    }
    while (tree->strings) {
        struct zz_string_chunk *next = tree->strings->next;
        for (i = 0; i < ZZ_CHUNK_SIZE; ++i)
            free(tree->strings->data[i]);
        free(tree->strings);
        tree->strings = next;
    }
}

const struct zz_node_type *zz_tree_token_type(struct zz_tree * tree, int tok)
{
    static const struct zz_node_type bad_token_type = { "<bad token>", -1 };
    if ((size_t)tok >= tree->token_types_size)
        return &bad_token_type;
    return &tree->token_types[tok];
}

const struct zz_node_type *zz_token_type(struct zz_node *node)
{
    return zz_tree_token_type(node->tree, node->token);
}

const char *zz_tree_token_name(struct zz_tree * tree, int tok)
{
    return zz_tree_token_type(tree, tok)->name;
}

const char *zz_token_name(struct zz_node * node)
{
    return zz_token_type(node)->name;
}

struct zz_node * zz_null(struct zz_tree * tree, int token)
{
    struct zz_node *node;
    assert(tree->token_types[token].type == ZZ_NULL);
    node = zz_node_alloc(tree);
    node->token = token;
    return node;
}

struct zz_node *zz_int(struct zz_tree *tree, int token, int data)
{
    struct zz_node *node;
    assert(tree->token_types[token].type == ZZ_INT);
    node = zz_node_alloc(tree);
    node->token = token;
    node->data.int_val = data;
    return node;
}

struct zz_node *zz_uint(struct zz_tree *tree, int token, unsigned int data)
{
    struct zz_node *node;
    assert(tree->token_types[token].type == ZZ_UINT);
    node = zz_node_alloc(tree);
    node->token = token;
    node->data.uint_val = data;
    return node;
}

struct zz_node *zz_double(struct zz_tree *tree, int token, double data)
{
    struct zz_node *node;
    assert(tree->token_types[token].type == ZZ_DOUBLE);
    node = zz_node_alloc(tree);
    node->token = token;
    node->data.double_val = data;
    return node;
}

struct zz_node *zz_string(struct zz_tree *tree, int token, const char *data)
{
    struct zz_node *node;
    assert(tree->token_types[token].type == ZZ_STRING);
    node = zz_node_alloc(tree);
    node->token = token;
    node->data.str_val = zz_string_alloc(tree, data);
    return node;
}

struct zz_node *zz_pointer(struct zz_tree *tree, int token, void *data)
{
    struct zz_node *node;
    assert(tree->token_types[token].type == ZZ_POINTER);
    node = zz_node_alloc(tree);
    node->token = token;
    node->data.ptr_val = data;
    return node;
}

struct zz_node *zz_inner(struct zz_tree *tree, int token, struct zz_list data)
{
    struct zz_node *node;
    assert(tree->token_types[token].type == ZZ_INNER);
    node = zz_node_alloc(tree);
    node->token = token;
    node->data.list_val = data;
    return node;
}

struct zz_node * zz_copy(struct zz_tree * tree, struct zz_node * node)
{
    const struct zz_node_type *tt = zz_token_type(node);

    switch (tt->type) {
    case ZZ_NULL:
        return zz_null(tree, node->token);
    case ZZ_INT:
        return zz_int(tree, node->token, node->data.int_val);
    case ZZ_UINT:
        return zz_uint(tree, node->token, node->data.uint_val);
    case ZZ_DOUBLE:
        return zz_double(tree, node->token, node->data.double_val);
    case ZZ_STRING:
        return zz_string(tree, node->token, node->data.str_val);
    case ZZ_POINTER:
        return zz_string(tree, node->token, node->data.ptr_val);
    case ZZ_INNER:
        return zz_inner(tree, node->token, zz_list_empty);
    }
}

struct zz_node * zz_copy_recursive(struct zz_tree * tree, struct zz_node * node)
{
    struct zz_node *ret;
    struct zz_node *it;

    ret = zz_copy(tree, node);

    if (tree->token_types[node->token].type == ZZ_INNER) {
        for (it = node->data.list_val.first; it; it = it->next)
            zz_append(ret, zz_copy_recursive(tree, it));
    }

    return ret;
}

int zz_to_int(const struct zz_node *node)
{
    assert(node->tree->token_types[node->token].type == ZZ_INT);
    return node->data.int_val;
}

unsigned int zz_to_uint(const struct zz_node *node)
{
    assert(node->tree->token_types[node->token].type == ZZ_UINT);
    return node->data.uint_val;
}

double zz_to_double(const struct zz_node *node)
{
    assert(node->tree->token_types[node->token].type == ZZ_DOUBLE);
    return node->data.double_val;
}

const char *zz_to_string(const struct zz_node *node)
{
    assert(node->tree->token_types[node->token].type == ZZ_STRING);
    return node->data.str_val;
}

void *zz_to_pointer(const struct zz_node *node)
{
    assert(node->tree->token_types[node->token].type == ZZ_POINTER);
    return node->data.ptr_val;
}

struct zz_node *zz_next(const struct zz_node *node)
{
    return node->next;
}

struct zz_node *zz_children(const struct zz_node *node)
{
    assert(node->tree->token_types[node->token].type == ZZ_INNER);
    return node->data.list_val.first;
}

void zz_clear(struct zz_node *node)
{
    assert(node->tree->token_types[node->token].type == ZZ_INNER);
    node->data.list_val = zz_list_empty;
}

void zz_append(struct zz_node *node, struct zz_node *child)
{
    assert(node->tree->token_types[node->token].type == ZZ_INNER);
    node->data.list_val = zz_list_append(node->data.list_val, child);
}

void zz_prepend(struct zz_node *node, struct zz_node *child)
{
    assert(node->tree->token_types[node->token].type == ZZ_INNER);
    node->data.list_val = zz_list_prepend(node->data.list_val, child);
}

void zz_insert(struct zz_node *node, struct zz_node *prev,
        struct zz_node *child)
{
    assert(node->tree->token_types[node->token].type == ZZ_INNER);
    node->data.list_val = zz_list_insert(node->data.list_val, prev, child);
}

void zz_append_list(struct zz_node *node, struct zz_list list)
{
    assert(node->tree->token_types[node->token].type == ZZ_INNER);
    node->data.list_val = zz_list_concat(node->data.list_val, list);
}

void zz_prepend_list(struct zz_node *node, struct zz_list list)
{
    assert(node->tree->token_types[node->token].type == ZZ_INNER);
    node->data.list_val = zz_list_concat(list, node->data.list_val);
}

struct zz_list zz_list(struct zz_node * t, ...)
{
    struct zz_list list;
    va_list ap;

    list.first = t;
    list.last = t;
    va_start(ap, t);
    while ((list.last->next = va_arg(ap, void *)))
        list.last = list.last->next;
    va_end(ap);
    return list;
}

struct zz_list zz_list_append(struct zz_list list,
        struct zz_node * t)
{
    if (list.last)
        list.last->next = t;
    else
        list.first = t;
    list.last = t;
    return list;
}

struct zz_list zz_list_prepend(struct zz_list list,
        struct zz_node * t)
{
    if (list.first)
        t->next = list.first;
    else
        list.last = t;
    list.first = t;
    return list;
}

struct zz_list zz_list_insert(struct zz_list list,
        struct zz_node * prev, struct zz_node * t)
{
    t->next = prev->next;
    prev->next = t;
    if (list.last == prev)
        list.last = t;
    return list;
}

struct zz_list zz_list_concat(struct zz_list left,
        struct zz_list right)
{
    if (left.first == NULL)
        return right;
    if (right.first == NULL)
        return left;
    left.last->next = right.first;
    left.last = right.last;
    return left;
}

struct zz_node * zz_list_index(struct zz_list list, int i)
{
    struct zz_node *ret;
    for (ret = list.first; ret && i > 0; --i, ret = ret->next)
        continue;
    return ret;
}

void zz_print_node(struct zz_node * node, FILE * f)
{
    const struct zz_node_type *tt = &node->tree->token_types[node->token];
    fprintf(f, "[%s", tt->name);

    switch (tt->type) {
        case ZZ_NULL:
            break;
        case ZZ_INT:
            fprintf(f, " %d", node->data.int_val);
            break;
        case ZZ_UINT:
            fprintf(f, " %u", node->data.uint_val);
            break;
        case ZZ_DOUBLE:
            fprintf(f, " %f", node->data.double_val);
            break;
        case ZZ_STRING:
            fprintf(f, " \"%s\"", node->data.str_val);
            break;
        case ZZ_POINTER:
            fprintf(f, " %p", node->data.ptr_val);
            break;
        case ZZ_INNER:
            zz_print_list(node->data.list_val, f);
            break;
    }

    fprintf(f, "]");
}

void zz_print_list(struct zz_list list, FILE * f)
{
    struct zz_node *it;

    if (it = list.first) {
        for (;;) {
            fprintf(f, " ");
            zz_print_node(it, f);
            if ((it = it->next) == NULL)
                break;
        }
    }
}

int zz_match_at_line(struct zz_node * node, int tok, const char *file, int line)
{
    if (node == NULL) {
        fprintf(stderr, "%s:%d: unexpected end of node list", file, line);
        return -1;
    } else if (node->token != tok) {
        fprintf(stderr, "%s:%d: expected %s, got %s", file, line,
                zz_tree_token_type(node->tree, tok)->name,
                zz_token_type(node)->name);
        return -1;
    }
    return 0;
}

int zz_match_end_at_line(struct zz_node * node, const char *file, int line)
{
    if (node != NULL) {
        fprintf(stderr, "%s:%d: unexpected node: %s",
                file, line, zz_token_type(node)->name);
        return -1;
    }
    return 0;
}
