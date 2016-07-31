

#include "zebu.h"

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

/*
 * A chunk of memory for a tree
 *
 * Just a little construct that allows us to allocate memory in chunks and only
 * deallocate it all at once, when the tree is destroyed.
 *
 * This is a header that goes before the actual memory blob, that is at least
 * ZZ_BLOB_SIZE bytes long.
 */
struct zz_blob {
	/* Next blob in list */
	struct zz_blob *next;
	/* Amount of memory used */
	size_t used;
};

/*
 * A list of all strings held by the tree
 *
 * This is basically a way to prevent the tree from allocating the same string
 * twice.
 *
 * Again, this is a header that goes before the actual string index, that has
 * enough bytes to hold alloc strings.
 */
struct zz_string_index {
	/* Allocated memory */
	size_t alloc;
	/* Used memory */
	size_t used;
};

const struct zz_list zz_list_empty = { NULL, NULL };

static void *zz_alloc(struct zz_tree *tree, size_t nbytes)
{
	struct zz_blob *blob;
	void *ptr;

	if (nbytes >= ZZ_BLOB_SIZE) {
		blob = calloc(1, sizeof(*blob) + nbytes);
		blob->next = tree->blobs;
		tree->blobs = blob;
	} else {
		blob = tree->blobs;
		while (blob != NULL && blob->used + nbytes > ZZ_BLOB_SIZE)
			blob = blob->next;
		if (blob == NULL) {
			blob = calloc(1, sizeof(*blob) + ZZ_BLOB_SIZE);
			blob->next = tree->blobs;
			tree->blobs = blob;
		}
	}
	ptr = (char *)(blob + 1) + blob->used;
	blob->used += nbytes;
	return ptr;
}

static struct zz_node *zz_alloc_node(struct zz_tree *tree)
{
	struct zz_node *node;

	node = zz_alloc(tree, sizeof(*node));
	node->tree = tree;
	if (tree->gen_loc != NULL)
		tree->gen_loc(&node->loc, tree->gen_loc_data);
	return node;
}

static const char *zz_alloc_string(struct zz_tree *tree, const char *str)
{
	struct zz_string_index *index;
	char **data;
	size_t i;
	size_t len;

	index = tree->strings;
	data = (char **)(index + 1);
	for (i = 0; i < index->used; ++i) {
		if (strcmp(data[i], str) == 0)
			return data[i];
	}
	if (index->used == index->alloc) {
		index->alloc = index->alloc ? index->alloc * 2 : 2;
		len = sizeof(*index) + index->alloc * sizeof(char **);
		index = realloc(index, len);
		tree->strings = index;
		data = (char **)(index + 1);
	}
	++index->used;
	data[i] = zz_alloc(tree, strlen(str) + 1);
	strcpy(data[i], str);
	return data[i];
}

void zz_tree_init(struct zz_tree *tree,
		const struct zz_node_type * token_types, size_t token_types_size)
{
	tree->token_types = token_types;
	tree->token_types_size = token_types_size;
	tree->blobs = NULL;
	tree->strings = calloc(1, sizeof(struct zz_string_index));
	tree->gen_loc = NULL;
	tree->gen_loc_data = NULL;
}

void zz_tree_deinit(struct zz_tree * tree)
{
	struct zz_blob *blob;
	struct zz_blob *next;

	blob = tree->blobs;
	while (blob != NULL) {
		next = blob->next;
		free(blob);
		blob = next;
	}

	free(tree->strings);
}

int zz_tree_token_type(struct zz_tree * tree, int tok)
{
	if ((size_t)tok >= tree->token_types_size)
		return -1;
	return tree->token_types[tok].type;
}

int zz_token_type(struct zz_node *node)
{
	return zz_tree_token_type(node->tree, node->token);
}

const char *zz_tree_token_name(struct zz_tree * tree, int tok)
{
	if ((size_t)tok >= tree->token_types_size)
		return "<bad>";
	return tree->token_types[tok].name;
}

const char *zz_token_name(struct zz_node * node)
{
	return zz_tree_token_name(node->tree, node->token);
}

struct zz_node * zz_null(struct zz_tree * tree, int token)
{
	struct zz_node *node;
	assert(tree->token_types[token].type == ZZ_NULL);
	node = zz_alloc_node(tree);
	node->token = token;
	return node;
}

struct zz_node *zz_int(struct zz_tree *tree, int token, int data)
{
	struct zz_node *node;
	assert(tree->token_types[token].type == ZZ_INT);
	node = zz_alloc_node(tree);
	node->token = token;
	node->data.int_val = data;
	return node;
}

struct zz_node *zz_uint(struct zz_tree *tree, int token, unsigned int data)
{
	struct zz_node *node;
	assert(tree->token_types[token].type == ZZ_UINT);
	node = zz_alloc_node(tree);
	node->token = token;
	node->data.uint_val = data;
	return node;
}

struct zz_node *zz_double(struct zz_tree *tree, int token, double data)
{
	struct zz_node *node;
	assert(tree->token_types[token].type == ZZ_DOUBLE);
	node = zz_alloc_node(tree);
	node->token = token;
	node->data.double_val = data;
	return node;
}

struct zz_node *zz_string(struct zz_tree *tree, int token, const char *data)
{
	struct zz_node *node;
	assert(tree->token_types[token].type == ZZ_STRING);
	node = zz_alloc_node(tree);
	node->token = token;
	node->data.str_val = zz_alloc_string(tree, data);
	return node;
}

struct zz_node *zz_pointer(struct zz_tree *tree, int token, void *data)
{
	struct zz_node *node;
	assert(tree->token_types[token].type == ZZ_POINTER);
	node = zz_alloc_node(tree);
	node->token = token;
	node->data.ptr_val = data;
	return node;
}

struct zz_node *zz_inner(struct zz_tree *tree, int token, struct zz_list data)
{
	struct zz_node *node;
	assert(tree->token_types[token].type == ZZ_INNER);
	node = zz_alloc_node(tree);
	node->token = token;
	node->data.list_val = data;
	return node;
}

struct zz_node * zz_copy(struct zz_tree * tree, struct zz_node * node)
{
	switch (zz_token_type(node)) {
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

void zz_replace(struct zz_node *node, struct zz_node *oldc, struct zz_node *newc)
{
	assert(node->tree->token_types[node->token].type == ZZ_INNER);
	node->data.list_val = zz_list_replace(node->data.list_val, oldc, newc);
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

struct zz_list zz_list_replace(struct zz_list list, struct zz_node *oldc,
		struct zz_node *newc)
{
	if (list.first == oldc) {
		newc->next = oldc->next;
		list.first = newc;
	} else {
		struct zz_node *prev;
		for (prev = list.first; prev != NULL; prev = prev->next) {
			if (prev->next == oldc) {
				newc->next = oldc->next;
				prev->next = newc;
				break;
			}
		}
	}
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

	for (it = list.first; it != NULL; it = zz_next(it)) {
		fprintf(f, " ");
		zz_print_node(it, f);
	}
}

int zz_match(struct zz_node *node, int tok)
{
	char buf[1024];

	if (node == NULL) {
		zz_node_error(node, "unexpected end of node list");
		return -1;
	} else if (node->token != tok) {
		snprintf(buf, sizeof(buf), "expected %s, got %s",
			zz_tree_token_name(node->tree, tok),
			zz_token_name(node));
		zz_node_error(node, buf);
		return -1;
	}
	return 0;
}

int zz_match_end(struct zz_node *node)
{
	if (node != NULL) {
		zz_node_error(node, "unexpected node");
		return -1;
	}
	return 0;
}

void zz_node_error(struct zz_node *node, const char *msg)
{
	zz_error(msg, node->loc.file, node->loc.line, node->loc.column);
}

void zz_error(const char *msg, const char *file, size_t line, size_t column)
{
	FILE *f;
	int i;
	int c;
	int r;

	if (file == NULL) {
		fprintf(stderr, "<file>:%d: %s\n", line, msg);
		return;
	}
	fprintf(stderr, "%s:%d: %s", file, line, msg);
	f = fopen(file, "r");
	if (f == NULL)
		return;
	fseek(f, 0, SEEK_SET);
	for (i = 1; i < line; ++i) {
		while (fgetc(f) != '\n')
			continue;
	}
	fprintf(stderr, "\n");
	r = ftell(f);
	for (i = 0; (c = fgetc(f)) != '\n'; ++i)
		fputc(c, stderr);
	fprintf(stderr, "\n");
	fseek(f, r, SEEK_SET);
	for (i = 1; i < column; ++i)
		fputc(fgetc(f) == '\t' ? '\t' : ' ', stderr);
	fprintf(stderr, "^\n");
	fclose(f);
}
