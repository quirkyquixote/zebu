/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */
/**
 * @file
 * Declare zz_tree struct and related functions
 */

#ifndef ZEBU_TREE_H_
#define ZEBU_TREE_H_

#include "node.h"

/**
 * Abstract Syntax Tree
 *
 * Not actually the tree, but a factory to produce new nodes that can
 * deallocate all them with a sigle call.
 *
 * @ingroup Zebu
 */
struct zz_tree {
	/** Size of each node */
	size_t node_size;
	/** All nodes managed by this tree */
	struct zz_list nodes;
};

/**
 * Initialize tree 
 *
 * @memberof zz_tree
 * @param [in] tree a zz_tree
 * @param [in] node_size size of node; it must be at least sizeof(struct zz_node)
 */
void zz_tree_init(struct zz_tree *tree, size_t node_size);
/**
 * Destroy tree 
 *
 * @memberof zz_tree
 * @param [in] tree a zz_tree
 */
void zz_tree_destroy(struct zz_tree *tree);

/**
 * Create a node 
 *
 * @memberof zz_tree
 * @param [in] tree a zz_tree
 * @param [in] tok a token
 * @param [in] data payload
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_node(struct zz_tree *tree, const char *tok, struct zz_data data);
/**
 * Destroy a node 
 *
 * @memberof zz_tree
 * @param [in] n a zz_node
 */
void zz_unref(struct zz_node *n);
/**
 * Copy a node 
 *
 * @memberof zz_tree
 * @param [in] tree a zz_tree
 * @param [in] node zz_node to copy
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_copy(struct zz_tree *tree, struct zz_node *node);
/**
 * Copy a node and all its children recursively 
 *
 * @memberof zz_tree
 * @param [in] tree a zz_tree
 * @param [in] node zz_node to copy
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_copy_recursive(struct zz_tree *tree, struct zz_node *node);

#endif         // ZEBU_TREE_H_
