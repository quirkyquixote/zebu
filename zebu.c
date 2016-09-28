

#include "zebu.h"

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

	node = zz_alloc(tree, tree->node_size);
	zz_list_init(&node->siblings);
	zz_list_init(&node->children);
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

void zz_tree_init(struct zz_tree *tree, size_t node_size)
{
	assert(node_size >= sizeof(struct zz_node));
	tree->node_size = node_size;
	tree->blobs = NULL;
	tree->strings = calloc(1, sizeof(struct zz_string_index));
}

void zz_tree_destroy(struct zz_tree * tree)
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

struct zz_node * zz_null(struct zz_tree * tree, const char *token)
{
	struct zz_node *node;
	node = zz_alloc_node(tree);
	node->token = token;
	node->type = ZZ_NULL;
	return node;
}

struct zz_node *zz_int(struct zz_tree *tree, const char *token, int data)
{
	struct zz_node *node;
	node = zz_alloc_node(tree);
	node->token = token;
	zz_int_init(tree, node, data);
	return node;
}

struct zz_node *zz_uint(struct zz_tree *tree, const char *token, unsigned int data)
{
	struct zz_node *node;
	node = zz_alloc_node(tree);
	node->token = token;
	zz_uint_init(tree, node, data);
	return node;
}

struct zz_node *zz_double(struct zz_tree *tree, const char *token, double data)
{
	struct zz_node *node;
	node = zz_alloc_node(tree);
	node->token = token;
	zz_double_init(tree, node, data);
	return node;
}

struct zz_node *zz_string(struct zz_tree *tree, const char *token, const char *data)
{
	struct zz_node *node;
	node = zz_alloc_node(tree);
	node->token = token;
	zz_string_init(tree, node, data);
	return node;
}

struct zz_node *zz_pointer(struct zz_tree *tree, const char *token, void *data)
{
	struct zz_node *node;
	node = zz_alloc_node(tree);
	node->token = token;
	zz_pointer_init(tree, node, data);
	return node;
}

void zz_null_init(struct zz_tree *tree, struct zz_node *node)
{
	node->type = ZZ_NULL;
}

void zz_int_init(struct zz_tree *tree, struct zz_node *node, int val)
{
	node->type = ZZ_INT;
	node->data.int_val = val;
}

void zz_uint_init(struct zz_tree *tree, struct zz_node *node, unsigned int val)
{
	node->type = ZZ_UINT;
	node->data.uint_val = val;
}

void zz_double_init(struct zz_tree *tree, struct zz_node *node, double val)
{
	node->type = ZZ_DOUBLE;
	node->data.double_val = val;
}

void zz_string_init(struct zz_tree *tree, struct zz_node *node, const char *val)
{
	node->type = ZZ_STRING;
	node->data.str_val = zz_alloc_string(tree, val);
}

void zz_pointer_init(struct zz_tree *tree, struct zz_node *node, void *val)
{
	node->type = ZZ_POINTER;
	node->data.ptr_val = val;
}

struct zz_node * zz_copy(struct zz_tree * tree, struct zz_node * node)
{
	switch (node->type) {
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
		return zz_pointer(tree, node->token, node->data.ptr_val);
	}
	return NULL;
}

struct zz_node * zz_copy_recursive(struct zz_tree * tree, struct zz_node * node)
{
	struct zz_node *ret;
	struct zz_node *iter;

	ret = zz_copy(tree, node);
	if (ret == NULL)
		return ret;
	zz_foreach_child(iter, node)
		zz_append_child(ret, zz_copy_recursive(tree, iter));
	return ret;
}

void zz_print(struct zz_node *node, FILE * f)
{
	struct zz_node *iter;

	fprintf(f, "[%s", node->token);

	if (node->type == ZZ_INT)
		fprintf(f, " %d", node->data.int_val);
	else if (node->type == ZZ_UINT)
		fprintf(f, " %u", node->data.uint_val);
	else if (node->type == ZZ_DOUBLE)
		fprintf(f, " %f", node->data.double_val);
	else if (node->type == ZZ_STRING)
		fprintf(f, " \"%s\"", node->data.str_val);
	else if (node->type == ZZ_POINTER)
		fprintf(f, " %p", node->data.ptr_val);

	zz_foreach_child(iter, node) {
		fprintf(f, " ");
		zz_print(iter, f);
	}

	fprintf(f, "]");
}

void zz_error(const char *msg, const char *file, size_t first_line,
		size_t first_column, size_t last_line, size_t last_column)
{
	FILE *f;
	int i;
	int c;
	int r;

	if (file == NULL) {
		fprintf(stderr, "<file>:%d: %s\n", first_line, msg);
		return;
	}
	fprintf(stderr, "%s:%d: %s", file, first_line, msg);
	f = fopen(file, "r");
	if (f == NULL)
		return;
	fseek(f, 0, SEEK_SET);
	for (i = 1; i < first_line; ++i) {
		while (fgetc(f) != '\n')
			continue;
	}
	fputc('\n', stderr);
	r = ftell(f);
	for (i = 0; (c = fgetc(f)) != '\n'; ++i)
		fputc(c, stderr);
	if (last_line > first_line)
		last_column = i - 1;
	fputc('\n', stderr);
	fseek(f, r, SEEK_SET);
	for (i = 1; i < first_column; ++i)
		fputc(fgetc(f) == '\t' ? '\t' : ' ', stderr);
	for (; i <= last_column; ++i)
		fputc(fgetc(f) == '\t' ? '\t' : '^', stderr);
	fputc('\n', stderr);
	fclose(f);
}
