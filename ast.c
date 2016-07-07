

#include <ctype.h>
#include <stdarg.h>

#include "ast.h"

struct ast_node_chunk {
    struct ast_node_chunk *next;
    struct ast_node data[AST_CHUNK_SIZE];
};

struct ast_string_chunk {
    struct ast_string_chunk *next;
    char *data[AST_CHUNK_SIZE];
}

struct ast_mgr {
    const struct ast_node_type *token_types;
    size_t token_types_size;
    struct ast_node_chunk *nodes;
    size_t node_chunk_size;
    char **strings;
    size_t strings_alloc;
    size_t strings_used;
};

const struct ast_list ast_list_empty = { NULL, NULL };

static struct ast_node *ast_node_alloc(struct ast_mgr *mgr)
{
    struct ast_node_chunk *chunk;

    if (mgr->node_chunk_size == AST_CHUNK_SIZE) {
        chunk = calloc(1, sizeof(*chunk));
        chunk->next = mgr->nodes;
        mgr->nodes = chunk;
        mgr->node_chunk_size = 0;
    }
    return mgr->nodes->data[mgr->node_chunk_size++];
}

static const char *str = ast_string_alloc(struct ast_mgr *mgr, const char *str)
{
    struct ast_string_chunk *chunk;
    int i;
    size_t len;
    char *buf;

    chunk = mgr->strings;
    for (i = 0; i < mgr->string_chunk_size; ++i) {
        if (strcmp(chunk->data[i], str) == 0)
            return chunk->data[i];
    }
    for (chunk = mgr->strings; chunk; chunk = chunk->next) {
        for (i = 0; i < AST_CHUNK_SIZE; ++i) {
            if (strcmp(chunk->data[i], str) == 0)
                return chunk->data[i];
        }
    }
    if (mgr->node_chunk_size == AST_CHUNK_SIZE) {
        chunk = calloc(1, sizeof(*chunk));
        chunk->next = mgr->strings;
        mgr->strings = chunk;
        mgr->string_chunk_size = 0;
    }
    len = strlen(str) + 1;
    buf = malloc(strlen(str));
    memcpy(buf, str, len);
    mgr->nodes->data[mgr->string_chunk_size++] = buf;
    return buf;
}

void ast_mgr_new(struct ast_mgr *mgr,
        const struct ast_node_type * token_types, size_t token_types_size)
{
    mgr->token_types = token_types;
    mgr->token_types_size = token_types_size;
    mgr->nodes = NULL;
    mgr->node_chunk_size = AST_CHUNK_SIZE;
    mgr->strings = NULL;
    mgr->string_chunk_size = AST_CHUNK_SIZE;
}

void ast_mgr_free(struct ast_mgr * mgr)
{
    int i;

    while (mgr->nodes) {
        struct ast_node_chunk *next = mgr->nodes->next;
        free(mgr->nodes);
        mgr->nodes = next;
    }
    if (mgr->strings) {
        struct ast_string_chunk *next = mgr->strings->next;
        for (i = 0; i < mgr->string_chunk_size; ++i)
            free(mgr->strings->data[i]);
        free(mgr->strings);
        mgr->strings = next;
    }
    while (mgr->strings) {
        struct ast_string_chunk *next = mgr->strings->next;
        for (i = 0; i < AST_CHUNK_SIZE; ++i)
            free(mgr->strings->data[i]);
        free(mgr->strings);
        mgr->strings = next;
    }
}

const struct ast_node_type *ast_mgr_token_type(struct ast_mgr * mgr, int tok)
{
    static const struct ast_node_type bad_token_type = { "<bad token>", -1 };
    if ((size_t)tok >= mgr->token_types_size)
        return &bad_token_type;
    return &mgr->token_types[tok];
}

const struct ast_node_type *ast_token_type(struct ast_node *ast)
{
    return ast_mgr_token_type(ast->mgr, ast->token);
}

const char *ast_mgr_token_name(struct ast_mgr * mgr, int tok)
{
    return ast_mgr_token_type(mgr, tok)->name;
}

const char *ast_token_name(struct ast_node * ast)
{
    return ast_token_type(ast)->name;
}

struct ast_node * ast_null(struct ast_mgr * mgr, int token)
{
    assert(mkr->token_types[token].type == AST_NULL);
    struct ast_node *node = ast_node_alloc(mgr);
    node->mgr = mgr;
    node->token = token;
    return node;
}

struct ast_node *ast_int(struct ast_mgr *mgr, int token, int data)
{
    assert(mgr->token_types[token].type == AST_INT);
    struct ast_node *node = ast_node_alloc(mgr);
    node->mgr = mgr;
    node->token = token;
    node->data.sint = data;
    return node;
}

struct ast_node *ast_uint(struct ast_mgr *mgr, int token, unsigned int data)
{
    assert(mgr->token_types[token].type == AST_UINT);
    struct ast_node *node = ast_node_alloc(mgr);
    node->mgr = mgr;
    node->token = token;
    node->data.uint = data;
    return node;
}

struct ast_node *ast_float(struct ast_mgr *mgr, int token, double data)
{
    assert(mgr->token_types[token].type == AST_FLOAT);
    struct ast_node *node = ast_node_alloc(mgr);
    node->mgr = mgr;
    node->token = token;
    node->data.real = data;
    return node;
}

struct ast_node *ast_string(struct ast_mgr *mgr, int token, const char *data)
{
    assert(mgr->token_types[token].type == AST_STRING);
    struct ast_node *node = ast_node_alloc(mgr);
    node->mgr = mgr;
    node->token = token;
    node->data.string = ast_string_alloc(mgr, data);
    return node;
}

struct ast_node *ast_inner(struct ast_mgr *mgr, int token, struct ast_list data)
{
    assert(mgr->token_types[token].type == AST_INNER);
    struct ast_node *node = ast_node_alloc(mgr);
    node->mgr = mgr;
    node->token = token;
    node->data.list = data;
    return node;
}

struct ast_node * ast_copy(struct ast_mgr * mgr, struct ast_node * ast)
{
    const struct ast_node_type *tt = ast_token_type(ast);

    switch (tt->type) {
    case FTU_NULL_AST:
        return ast_new(mgr, ast->token);
    case FTU_INT_AST:
        return ast_int(mgr, ast->token, ast->data.sint);
    case FTU_UINT_AST:
        return ast_uint(mgr, ast->token, ast->data.uint);
    case FTU_FLOAT_AST:
        return ast_float(mgr, ast->token, ast->data.real);
    case FTU_STRING_AST:
        return ast_string(mgr, ast->token, ast->data.string);
    case FTU_INNER_AST:
        return ast_inner(mgr, ast->token, ast_list_empty);
    }
}

struct ast_node * ast_copy_recursive(struct ast_mgr * mgr, struct ast_node * ast)
{
    struct ast_node *ret;
    struct ast_node *it;

    ret = ast_copy(mgr, ast);

    if (mgr->token_types[ast->token].type == FTU_INNER_AST) {
        for (it = ast->data.list.first; it; it = it->next)
            inner_ast_append(ret, ast_copy_recursive(mgr, it));
    }

    return ret;
}

int ast_to_int(const struct ast_node *ast)
{
    assert(ast->mgr->token_types[ast->token].type == AST_INT);
    return ast->data.sint;
}

unsigned int ast_to_uint(const struct ast_node *ast)
{
    assert(ast->mgr->token_types[ast->token].type == AST_UINT);
    return ast->data.uint;
}

double ast_to_float(const struct ast_node *ast)
{
    assert(ast->mgr->token_types[ast->token].type == AST_FLOAT);
    return ast->data.real;
}

const char *ast_to_string(const struct ast_node *ast)
{
    assert(ast->mgr->token_types[ast->token].type == AST_STRING);
    return ast->data.string;
}

struct ast_list ast_to_list(const struct ast_node *ast)
{
    assert(ast->mgr->token_types[ast->token].type == AST_INNER);
    return ast->data.list;
}

void ast_clear(struct ast_node *ast)
{
    assert(ast->mgr->token_types[ast->token].type == AST_INNER);
    ast->data.list = ast_list_empty;
}

void ast_append(struct ast_node *ast, struct ast_node *child)
{
    assert(ast->mgr->token_types[ast->token].type == AST_INNER);
    ast->data.list = ast_list_append(ast->data.list, child);
}

void ast_prepend(struct ast_node *ast, struct ast_node *child)
{
    assert(ast->mgr->token_types[ast->token].type == AST_INNER);
    ast->data.list = ast_list_prepend(ast->data.list, child);
}

void ast_insert(struct ast_node *ast, struct ast_node *prev,
        struct ast_node *child)
{
    assert(ast->mgr->token_types[ast->token].type == AST_INNER);
    ast->data.list = ast_list_insert(ast->data.list, prev, child);
}

void ast_append_list(struct ast_node *ast, struct ast_list children)
{
    assert(ast->mgr->token_types[ast->token].type == AST_INNER);
    ast->data.list = ast_list_concat(ast->data.list, children);
}

void ast_prepend_list(struct ast_node *ast, struct ast_list children)
{
    assert(ast->mgr->token_types[ast->token].type == AST_INNER);
    ast->data.list = ast_list_concat(children, ast->data.listn);
}

struct ast_list ast_list(struct ast_node * t, ...)
{
    struct ast_list list = { t, t };
    va_list ap;

    va_start(ap, t);
    while ((list.last->next = va_arg(ap, void *)))
        list.last = list.last->next;
    va_end(ap);
    return list;
}

struct ast_list ast_list_append(struct ast_list list,
        struct ast_node * t)
{
    if (list.last)
        list.last->next = t;
    else
        list.first = t;
    list.last = t;
    return list;
}

struct ast_list ast_list_prepend(struct ast_list list,
        struct ast_node * t)
{
    if (list.first)
        t->next = list.first;
    else
        list.last = t;
    list.first = t;
    return list;
}

struct ast_list ast_list_insert(struct ast_list list,
        struct ast_node * prev, struct ast_node * t)
{
    t->next = prev->next;
    prev->next = t;
    if (list.last == prev)
        list.last = t;
    return list;
}

struct ast_list ast_list_concat(struct ast_list left,
        struct ast_list right)
{
    if (left.first == NULL)
        return right;
    if (right.first == NULL)
        return left;
    left.last->next = right.first;
    left.last = right.last;
    return left;
}

struct ast_node * ast_list_index(struct ast_list list, int i)
{
    struct ast_node *ret;
    for (ret = list.first; ret && i > 0; --i, ret = ret->next)
        continue;
    return ret;
}

