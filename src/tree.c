

#include "tree.h"

#include <ctype.h>
#include <stdarg.h>
#include <string.h>

#include "dict.h"

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
	zz_int_init(node, data);
	return node;
}

struct zz_node *zz_uint(struct zz_tree *tree, const char *token, unsigned int data)
{
	struct zz_node *node;
	node = zz_alloc_node(tree);
	node->token = token;
	zz_uint_init(node, data);
	return node;
}

struct zz_node *zz_double(struct zz_tree *tree, const char *token, double data)
{
	struct zz_node *node;
	node = zz_alloc_node(tree);
	node->token = token;
	zz_double_init(node, data);
	return node;
}

struct zz_node *zz_string(struct zz_tree *tree, const char *token, const char *data)
{
	struct zz_node *node;
	node = zz_alloc_node(tree);
	node->token = token;
	zz_string_init(node, zz_alloc_string(tree, data));
	return node;
}

struct zz_node *zz_pointer(struct zz_tree *tree, const char *token, void *data)
{
	struct zz_node *node;
	node = zz_alloc_node(tree);
	node->token = token;
	zz_pointer_init(node, data);
	return node;
}

void zz_set_null(struct zz_tree * tree, struct zz_node *node)
{
	node->type = ZZ_NULL;
}

void zz_set_int(struct zz_tree *tree, struct zz_node *node, int data)
{
	zz_int_init(node, data);
}

void zz_set_uint(struct zz_tree *tree, struct zz_node *node, unsigned int data)
{
	zz_uint_init(node, data);
}

void zz_set_double(struct zz_tree *tree, struct zz_node *node, double data)
{
	zz_double_init(node, data);
}

void zz_set_string(struct zz_tree *tree, struct zz_node *node, const char *data)
{
	zz_string_init(node, zz_alloc_string(tree, data));
}

void zz_set_pointer(struct zz_tree *tree, struct zz_node *node, void *data)
{
	zz_pointer_init(node, data);
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

