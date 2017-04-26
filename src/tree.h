
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
	struct zz_list nodes;
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
 * Create a node 
 *
 * @tree a zz_tree
 * @tok a token
 * @data payload
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_node(struct zz_tree *tree, const char *tok, struct zz_data data);
/*
 * Create a string
 *
 * @tree a zz_tree
 * @str a string
 * @return a new string allocated by _tree_
 */
struct zz_data zz_string(struct zz_tree *tree, const char *str);
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

#endif         // ZEBU_TREE_H_
