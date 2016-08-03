

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
	node->tree = tree;
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

void zz_tree_init(struct zz_tree *tree, size_t node_size,
		const char *const *token_names, size_t num_tokens)
{
	assert(node_size >= sizeof(struct zz_node));
	tree->token_names = token_names;
	tree->num_tokens = num_tokens;
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

const char *zz_tree_token_name(struct zz_tree * tree, int tok)
{
	if ((size_t)tok >= tree->num_tokens)
		return NULL;
	return tree->token_names[tok];
}

const char *zz_token_name(struct zz_node * node)
{
	return zz_tree_token_name(node->tree, node->token);
}

struct zz_node * zz_null(struct zz_tree * tree, int token)
{
	struct zz_node *node;
	node = zz_alloc_node(tree);
	node->token = token;
	node->type = ZZ_NULL;
	return node;
}

struct zz_node *zz_int(struct zz_tree *tree, int token, int data)
{
	struct zz_node *node;
	node = zz_alloc_node(tree);
	node->token = token;
	node->type = ZZ_INT;
	node->data.int_val = data;
	return node;
}

struct zz_node *zz_uint(struct zz_tree *tree, int token, unsigned int data)
{
	struct zz_node *node;
	node = zz_alloc_node(tree);
	node->token = token;
	node->type = ZZ_UINT;
	node->data.uint_val = data;
	return node;
}

struct zz_node *zz_double(struct zz_tree *tree, int token, double data)
{
	struct zz_node *node;
	node = zz_alloc_node(tree);
	node->token = token;
	node->type = ZZ_DOUBLE;
	node->data.double_val = data;
	return node;
}

struct zz_node *zz_string(struct zz_tree *tree, int token, const char *data)
{
	struct zz_node *node;
	node = zz_alloc_node(tree);
	node->token = token;
	node->type = ZZ_STRING;
	node->data.str_val = zz_alloc_string(tree, data);
	return node;
}

struct zz_node *zz_pointer(struct zz_tree *tree, int token, void *data)
{
	struct zz_node *node;
	node = zz_alloc_node(tree);
	node->token = token;
	node->type = ZZ_POINTER;
	node->data.ptr_val = data;
	return node;
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
		return zz_string(tree, node->token, node->data.ptr_val);
	}
	return NULL;
}

struct zz_node * zz_copy_recursive(struct zz_tree * tree, struct zz_node * node)
{
	struct zz_node *ret;
	struct zz_list *iter;
	struct zz_node *tmp;

	ret = zz_copy(tree, node);
	if (ret == NULL)
		return ret;
	zz_list_foreach(iter, &node->children) {
		tmp = zz_copy_recursive(tree, (void *)iter);
		zz_append(&ret->children, &tmp->siblings);
	}
	return ret;
}

void zz_print_node(struct zz_node *node, FILE * f)
{
	fprintf(f, "[%s", zz_token_name(node));

	switch (node->type) {
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
	}

	zz_print_list(&node->children, f);
	fprintf(f, "]");
}

void zz_print_list(struct zz_list *list, FILE * f)
{
	struct zz_list *iter;
	zz_list_foreach(iter, list) {
		fprintf(f, " ");
		zz_print_node((void *)iter, f);
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

int zz_match_end(struct zz_node *node, struct zz_list *list)
{
	if (node != (void *)list) {
		node->tree->error(node, "unexpected node");
		return -1;
	}
	return 0;
}

void zz_node_error(struct zz_node *node, const char *msg)
{
	node->tree->error(node, msg);
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
