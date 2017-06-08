/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */

#ifndef ZEBU_TREE_H_
#define ZEBU_TREE_H_

#include "node.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Tree
 * ----
 */

/**
 * Abstract Syntax Tree
 *
 * Not actually the tree, but a factory to produce new nodes that can
 * deallocate all them with a sigle call.
 */
struct zz_tree {
	size_t node_size;
	struct zz_list nodes;
};

/**
 * Initialize tree 
 */
void zz_tree_init(struct zz_tree *tree, size_t node_size);
/**
 * Destroy tree 
 */
void zz_tree_destroy(struct zz_tree *tree);

/**
 * Create a node 
 */
struct zz_node *zz_node(struct zz_tree *tree, const char *tok, struct zz_data data);
/**
 * Destroy a node 
 */
void zz_unref(struct zz_node *n);
/**
 * Copy a node 
 */
struct zz_node *zz_copy(struct zz_tree *tree, struct zz_node *node);
/**
 * Copy a node and all its children recursively 
 */
struct zz_node *zz_copy_recursive(struct zz_tree *tree, struct zz_node *node);

#ifdef __cplusplus
}
#endif

#endif         // ZEBU_TREE_H_
