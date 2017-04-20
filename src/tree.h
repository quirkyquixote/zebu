
#ifndef ZEBU_TREE_H_
#define ZEBU_TREE_H_

#include "node.h"

/*
 * Abstract Syntax Tree
 *
 * Includes not only the tree structure--in fact, it doesn't even know which
 * node is the root--, but knows node types and names, and is the factory for
 * producing new zz_node and strings.
 *
 */
struct zz_tree {
	/* Size of each node */
	size_t node_size;
	/* All nodes managed by this tree */
	struct zz_node *nodes;
	/* Index of all strings managed by the tree */
	void *strings;
};

/*
 * Initialize tree 
 *
 * @tree a zz_tree
 * @node_size size of node; it must be at least sizeof(struct zz_node)
 */
void zz_tree_init(struct zz_tree *tree, size_t node_size);
/*
 * Destroy tree 
 *
 * @tree a zz_tree
 */
void zz_tree_destroy(struct zz_tree *tree);

/*
 * Copy a node 
 *
 * @tree a zz_tree
 * @node zz_node to copy
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_copy(struct zz_tree *tree, struct zz_node *node);
/*
 * Copy a node and all its children recursively 
 *
 * @tree a zz_tree
 * @node zz_node to copy
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_copy_recursive(struct zz_tree *tree, struct zz_node *node);

/*
 * Create an empty node 
 *
 * @tree a zz_tree
 * @tok a token
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_null(struct zz_tree *tree, const char *tok);
/*
 * Create a node with int data 
 *
 * @tree a zz_tree
 * @tok a token
 * @val an integer
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_int(struct zz_tree *tree, const char *tok, int val);
/*
 * Create a node with unsigned int data 
 *
 * @tree a zz_tree
 * @tok a token
 * @val an unsigned integer
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_uint(struct zz_tree *tree, const char *tok, unsigned int val);
/*
 * Create a node with double data 
 *
 * @tree a zz_tree
 * @tok a token
 * @val a floating point number
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_double(struct zz_tree *tree, const char *tok, double val);
/*
 * Create a node with string data 
 *
 * @tree a zz_tree
 * @tok a token
 * @val a __NULL_ terminated string
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_string(struct zz_tree *tree, const char *tok, const char *val);
/*
 * Create a node with user-managed data data 
 *
 * @tree a zz_tree
 * @tok a token
 * @val a pointer
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_pointer(struct zz_tree *tree, const char *tok, void *val);

/*
 * Reset node data to null
 *
 * @tree a zz_tree
 * @node a zz_node
 */
void zz_set_null(struct zz_tree *tree, struct zz_node *node);
/*
 * Reset node data to int
 *
 * @tree a zz_tree
 * @node a zz_node
 * @data new data for node
 */
void zz_set_int(struct zz_tree *tree, struct zz_node *node, int data);
/*
 * Reset node data to uint
 *
 * @tree a zz_tree
 * @node a zz_node
 * @data new data for node
 */
void zz_set_uint(struct zz_tree *tree, struct zz_node *node, unsigned int data);
/*
 * Reset node data to double
 *
 * @tree a zz_tree
 * @node a zz_node
 * @data new data for node
 */
void zz_set_double(struct zz_tree *tree, struct zz_node *node, double data);
/*
 * Reset node data to string
 *
 * @tree a zz_tree
 * @node a zz_node
 * @data new data for node
 */
void zz_set_string(struct zz_tree *tree, struct zz_node *node, const char *data);
/*
 * Reset node data to pointer
 *
 * @tree a zz_tree
 * @node a zz_node
 * @data new data for node
 */
void zz_set_pointer(struct zz_tree *tree, struct zz_node *node, void *data);

#endif         // ZEBU_TREE_H_
