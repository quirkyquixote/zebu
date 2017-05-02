/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */

#include "tree.h"

#include <ctype.h>
#include <stdarg.h>
#include <string.h>

void zz_tree_init(struct zz_tree *tree, size_t node_size)
{
	assert(node_size >= sizeof(struct zz_node));
	tree->node_size = node_size;
	zz_list_init(&tree->nodes);
}

void zz_tree_destroy(struct zz_tree * tree)
{
	struct zz_node *n, *x;
	zz_list_foreach_entry_safe(n, x, &tree->nodes, allocated) {
		zz_data_destroy(n->data);
		free(n);
	}
}

struct zz_node *zz_node(struct zz_tree * tree, const char *token, struct zz_data data)
{
	struct zz_node *n = calloc(1, tree->node_size);
	zz_list_init(&n->children);
	zz_list_init(&n->siblings);
	zz_list_init(&n->allocated);
	n->token = token;
	zz_list_append(&tree->nodes, &n->allocated);
	n->data = data;
	return n;
}

struct zz_node *zz_copy(struct zz_tree *tree, struct zz_node *node)
{
	return zz_node(tree, node->token, zz_data_copy(node->data));
}

struct zz_node * zz_copy_recursive(struct zz_tree * tree, struct zz_node * node)
{
	struct zz_node *ret;
	struct zz_node *iter;

	ret = zz_copy(tree, node);
	if (ret == NULL)
		return ret;
	zz_list_foreach_entry(iter, &node->children, siblings)
		zz_append_child(ret, zz_copy_recursive(tree, iter));
	return ret;
}

