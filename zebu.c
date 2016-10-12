

#include "zebu.h"

#include <ctype.h>
#include <stdarg.h>
#include <string.h>

/*
 * AA Tree to provide a dictionary of strings allocated by an AST.
 *
 * An AA Tree is a simplified form of red-black tree that implements a balanced
 * binary tree. We use an even more simplified form of it to keep an easily
 * accessible index of all strings belonging to an AST.
 *
 * This implementation lacks specialized lookup and deletion routines: lookup
 * is provided by the insert method--attempting to insert an already existing
 * string will return the already allocated data--, and elements can't be
 * deleted individually, but are destroyed all at once with the AST they belong
 * to.
 */
struct zz_dict {
	/* Left and right children */
	struct zz_dict *left, *right;
	/* Level of this node */
	size_t level;
	/* Data attached to this node */
	char data[];
};

static struct zz_dict *zz_dict_skew(struct zz_dict *t)
{
	struct zz_dict *l;

	if (t == NULL) {
		return NULL;
	} else if (t->left == NULL) {
		return t;
	} else if (t->left->level == t->level) {
		l = t->left;
		t->left = l->right;
		l->right = t;
		return l;
	} else {
		return t;
	}
}

static struct zz_dict *zz_dict_split(struct zz_dict *t)
{
	struct zz_dict *r;

	if (t == NULL) {
		return NULL;
	} else if (t->right == NULL || t->right->right == NULL) {
		return t;
	} else if (t->level == t->right->right->level) {
		r = t->right;
		t->right = r->left;
		r->left = t;
		++r->level;
		return r;
	} else {
		return t;
	}
}

static struct zz_dict *zz_dict_insert(struct zz_dict *t, const char *data,
		const char **rval)
{
	int cmp;

	if (t == NULL) {
		t = calloc(1, sizeof(*t) + strlen(data) + 1);
		t->level = 1;
		strcpy(t->data, data);
		if (rval != NULL)
			*rval = t->data;
		return t;
	}
	cmp = strcmp(data, t->data);
	if (cmp < 0) {
		t->left = zz_dict_insert(t->left, data, rval);
	} else if (cmp > 0) {
		t->right = zz_dict_insert(t->right, data, rval);
	} else {
		if (rval != NULL)
			*rval = t->data;
	}
	t = zz_dict_skew(t);
	t = zz_dict_split(t);
	return t;
}

static void zz_dict_destroy(struct zz_dict *t)
{
	if (t != NULL) {
		zz_dict_destroy(t->left);
		zz_dict_destroy(t->right);
		free(t);
	}
}

static struct zz_node *zz_alloc_node(struct zz_tree *tree)
{
	struct zz_node *node;
	node = calloc(1, tree->node_size);
	node->next = tree->nodes;
	zz_list_init(&node->siblings);
	zz_list_init(&node->children);
	tree->nodes = node;
	return node;
}

static const char *zz_alloc_string(struct zz_tree *tree, const char *str)
{
	const char *rval;
	tree->strings = zz_dict_insert(tree->strings, str, &rval);
	return rval;
}

void zz_tree_init(struct zz_tree *tree, size_t node_size)
{
	assert(node_size >= sizeof(struct zz_node));
	tree->node_size = node_size;
	tree->nodes = NULL;
	tree->strings = NULL;
}

void zz_tree_destroy(struct zz_tree * tree)
{
	struct zz_node *node;
	struct zz_node *next;
	for (node = tree->nodes; node != NULL; node = next) {
		next = node->next;
		free(node);
	}
	zz_dict_destroy(tree->strings);
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
