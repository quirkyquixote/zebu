
#ifndef ZEBU_H_
#define ZEBU_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/**
 * @defgroup Zebu
 * @{
 * Abstract Syntax Tree
 * @}
 */

/**
 * Minimum amount of memory to allocate at once
 * @ingroup Zebu
 */
#define ZZ_BLOB_SIZE 1024

/** 
 * Data types that may be held by a zz_node
 * @ingroup Zebu
 */
enum {
	ZZ_NULL,                    /**< Node with no information */
	ZZ_INT,                     /**< Node with an integer */
	ZZ_UINT,                    /**< Node with an unsigned integer */
	ZZ_DOUBLE,                  /**< Node with a floating point number */
	ZZ_STRING,                  /**< Node with a string */
	ZZ_POINTER                  /**< Node with user-managed data */
};

/**
 * List of zz_node
 * @ingroup Zebu
 */
struct zz_list {
	/** First node in the list */
	struct zz_node *first;
	/** Last node in the list */
	struct zz_node *last;
};

/**
 * Data that may be held by a node in the AST
 * @ingroup Zebu
 */
union zz_node_data {
	/** For ZZ_UINT */
	unsigned int uint_val;
	/** For ZZ_INT */
	int int_val;
	/** For ZZ_DOUBLE */
	double double_val;
	/** For ZZ_STRING */
	const char *str_val;
	/** For ZZ_POINTER */
	void *ptr_val;
};

/**
 * Location in a file from which a node was parsed
 * @ingroup Zebu
 */
struct zz_location {
	/** Name of the file */
	const char *file;
	/** Line number from where this node was parsed */
	size_t line;
	/** Column number from where this node was parsed */
	size_t column;
};

/**
 * Node in an Abstract Syntax Tree 
 * @ingroup Zebu
 */
struct zz_node {
	/** Tree to which this node belongs */
	struct zz_tree *tree;
	/** Next sibling */
	struct zz_node *next;
	/** Children */
	struct zz_list children;
	/** Token for this node */
	int token;
	/** Type of data held by the node */
	int type;
	/** Data that depends on the node type */
	union zz_node_data data;
};

/**
 * Abstract Syntax Tree
 *
 * Includes not only the tree structure--in fact, it doesn't even know which
 * node is the root--, but knows node types and names, and is the factory for
 * producing new zz_node and strings.
 *
 * @ingroup Zebu
 */
struct zz_tree {
	/** All node names for this tree */
	const char *const *token_names;
	/** Size of _token_names */
	size_t num_tokens;
	/** Size of each node */
	size_t node_size;
	/** All memory managed by this tree */
	void *blobs;
	/** Index of all strings managed by the tree */
	void *strings;
	/** Function to report an error */
	void (* error)(void *, const char *);
};

/**
 * Initialize tree 
 *
 * @memberof zz_tree
 * @param tree a zz_tree
 * @param node_size size of node; it must be at least sizeof(struct zz_node)
 * @param token_names all node names
 * @param num_tokens number of elements in _token_names_
 */
void zz_tree_init(struct zz_tree *tree, size_t node_size,
		const char *const *token_names, size_t num_tokens);
/**
 * Destroy tree 
 *
 * @memberof zz_tree
 * @param tree a zz_tree
 */
void zz_tree_deinit(struct zz_tree *tree);

/**
 * Get node type for token in tree, "<bad>" if nonexistant 
 *
 * @memberof zz_tree
 * @param tree a zz_tree
 * @param tok a valid token for _tree_
 * @return type of _tok_, or __NULL__
 */
int zz_tree_token_type(struct zz_tree *tree, int tok);
/**
 * Get node type for node 
 *
 * @memberof zz_node
 * @param node a zz_node
 * @return type of the token of _node_, or __NULL__
 */
int zz_token_type(struct zz_node *node);
/**
 * Get token name for token, or "<bad>" if unexistant 
 *
 * @memberof zz_tree
 * @param tree a zz_tree
 * @param tok a valid token for _tree_
 * @return name of _tok_, or __NULL__
 */
const char *zz_tree_token_name(struct zz_tree *tree, int tok);
/**
 * Get token name for node 
 *
 * @memberof zz_node
 * @param node a zz_node
 * @return name of the token of _node_, or __NULL__
 */
const char *zz_token_name(struct zz_node *node);

/**
 * Copy a node 
 *
 * @memberof zz_tree
 * @param tree a zz_tree
 * @param node zz_node to copy
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_copy(struct zz_tree *tree, struct zz_node *node);
/**
 * Copy a node and all its children recursively 
 *
 * @memberof zz_tree
 * @param tree a zz_tree
 * @param node zz_node to copy
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_copy_recursive(struct zz_tree *tree, struct zz_node *node);

/**
 * Create an empty node 
 *
 * @memberof zz_tree
 * @param tree a zz_tree
 * @param tok a token
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_null(struct zz_tree *tree, int tok);
/**
 * Create a node with int data 
 *
 * @memberof zz_tree
 * @param tree a zz_tree
 * @param tok a token
 * @param val an integer
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_int(struct zz_tree *tree, int tok, int val);
/**
 * Create a node with unsigned int data 
 *
 * @memberof zz_tree
 * @param tree a zz_tree
 * @param tok a token
 * @param val an unsigned integer
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_uint(struct zz_tree *tree, int tok, unsigned int val);
/**
 * Create a node with double data 
 *
 * @memberof zz_tree
 * @param tree a zz_tree
 * @param tok a token
 * @param val a floating point number
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_double(struct zz_tree *tree, int tok, double val);
/**
 * Create a node with string data 
 *
 * @memberof zz_tree
 * @param tree a zz_tree
 * @param tok a token
 * @param val a __NULL_ terminated string
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_string(struct zz_tree *tree, int tok, const char *val);
/**
 * Create a node with user-managed data data 
 *
 * @memberof zz_tree
 * @param tree a zz_tree
 * @param tok a token
 * @param val a pointer
 * @return a new zz_node allocated by _tree_
 */
struct zz_node *zz_pointer(struct zz_tree *tree, int tok, void *val);

/**
 * Return integer data associated with node 
 *
 * @memberof zz_node
 * @param node a zz_node
 * @return integer value associated with _node_
 */
int zz_to_int(const struct zz_node *node);
/**
 * Return unsigned integer data associated with node 
 *
 * @memberof zz_node
 * @param node a zz_node
 * @return unsigned integer value associated with _node_
 */
unsigned int zz_to_uint(const struct zz_node *node);
/**
 * Return float data associated with node 
 *
 * @memberof zz_node
 * @param node a zz_node
 * @return floating point value associated with _node_
 */
double zz_to_double(const struct zz_node *node);
/**
 * Return string data associated with node 
 *
 * @memberof zz_node
 * @param node a zz_node
 * @return string value associated with _node_
 */
const char *zz_to_string(const struct zz_node *node);
/**
 * Return pointer data associated with node 
 *
 * @memberof zz_node
 * @param node a zz_node
 * @return pointer value associated with _node_
 */
void *zz_to_pointer(const struct zz_node *node);

/**
 * Return next sibling 
 *
 * @memberof zz_node
 * @param node a zz_node
 * @return pointer to the next sibling of _node_, or __NULL__
 */
struct zz_node *zz_next(const struct zz_node *node);
/**
 * Return first child 
 *
 * @memberof zz_node
 * @param node a zz_node
 * @return pointer to the first child of _node_, or __NULL__
 */
struct zz_node *zz_children(const struct zz_node *node);
/**
 * Remove all children from node 
 *
 * @memberof zz_node
 * @param node a zz_node
 */
void zz_clear(struct zz_node *node);
/**
 * Append child to node 
 *
 * @memberof zz_node
 * @param node a zz_node
 * @param child a zz_node to append to _node_'s child list
 */
void zz_append(struct zz_node *t, struct zz_node *child);
/**
 * Prepend child to node 
 *
 * @memberof zz_node
 * @param node a zz_node
 * @param child a zz_node to prepend to _node_'s child list
 */
void zz_prepend(struct zz_node *t, struct zz_node *child);
/**
 * Insert child after prev in node 
 *
 * @memberof zz_node
 * @param node a zz_node
 * @param prev a child of _node_
 * @param child a zz_node to insert in _node_'s child list
 */
void zz_insert(struct zz_node *t, struct zz_node *prev, struct zz_node *child);
/**
 * Replace oldc by newc 
 *
 * @memberof zz_node
 * @param node a zz_node
 * @param oldc a child of _node_
 * @param newc a zz_node to swap by _oldc_ in _node_'s child list
 */
void zz_replace(struct zz_node *p, struct zz_node *oldc, struct zz_node *newc);
/**
 * Append all nodes in list to node 
 *
 * @memberof zz_node
 * @param node a zz_node
 * @oaram list a zz_list to append to _node_'s child list
 */
void zz_append_list(struct zz_node *t, struct zz_list list);
/**
 * Prepend all nodes in list to node 
 *
 * @memberof zz_node
 * @param node a zz_node
 * @oaram list a zz_list to prepend to _node_'s child list
 */
void zz_prepend_list(struct zz_node *t, struct zz_list list);

/**
 * Copy to initialize empty lists 
 */
extern const struct zz_list zz_list_empty;
/**
 * Create a list from the NULL terminated argument list 
 *
 * @memberof zz_list
 * @param list a zz_list
 * @param node a zz_node to be the first in the new list
 * @param ... a __NULL__ terminated list of more nodes
 * @return a zz_list object with all nodes
 */
struct zz_list zz_list(struct zz_node *t, ...);
/**
 * Append node to list
 *
 * Return the list that results from appending _node_ to _list_;
 * the original list is invalidated 
 *
 * @memberof zz_list
 * @param list a zz_list
 * @param node a zz_node
 * @return a zz_list 
 */
struct zz_list zz_list_append(struct zz_list list, struct zz_node *node);
/**
 * Prepend node to list
 * 
 * Return the list that results from prepending _node_ to _list_;
 * the original list is invalidated 
 *
 * @memberof zz_list
 * @param list a zz_list
 * @param node a zz_node
 * @return a zz_list
 */
struct zz_list zz_list_prepend(struct zz_list list, struct zz_node *node);
/**
 * Insert node in list
 * 
 * Return the list that results from inserting _node_ after _prev_ in _list_;
 * the original list is invalidated 
 *
 * @memberof zz_list
 * @param list a zz_list
 * @param pref a node in _list_
 * @param node a node to insert after _prev_ 
 * @return a zz_list
 */
struct zz_list zz_list_insert(struct zz_list list, struct zz_node *prev,
		struct zz_node *t);
/**
 * Replace node in list
 *
 * Return the list that results from replacing _oldc_ by _newc_ in _list_;
 * the original list is invalidated 
 *
 * @memberof zz_list
 * @param list a zz_list
 * @param oldc child of _list_ to replace
 * @param newc node to replace _oldc_
 * @return a zz_list
 */
struct zz_list zz_list_replace(struct zz_list list, struct zz_node *oldc,
		struct zz_node *newc);
/**
 * Concatenate lists
 *
 * Return the list that results from concatenating _left_ and _right_;
 * both left and right are invalidated 
 *
 * @memberof zz_list
 * @param left a zz_list
 * @param right a zz_list
 * @return a zz_list
 */
struct zz_list zz_list_concat(struct zz_list left, struct zz_list right);
/**
 * Return the element of list whose index is i 
 *
 * @memberof zz_list
 * @param list a zz_list
 * @param i index of the desired element in _list_
 * @return _i_th node in _list_, or __NULL__
 */
struct zz_node *zz_list_index(struct zz_list list, int i);

/**
 * Print the full tree whose root is _node_ to _f_ 
 *
 * @memberof zz_node
 * @param node a zz_node
 * @param f a FILE
 */
void zz_print_node(struct zz_node * node, FILE * f);
/**
 * Print the full forest whose roots are _list_ to _f_ 
 *
 * @memberof zz_list
 * @param list a zz_list
 * @param f a FILE
 */
void zz_print_list(struct zz_list list, FILE * f);

/**
 * Match node
 *
 * If the token for _node_ is not _tok_, call zz_node_error() and return -1.
 *
 * @memberof zz_node
 * @param node a zz_node
 * @param tok a token
 * @return 0 on success, -1 on failure
 */
int zz_match(struct zz_node * node, int tok);
/**
 * Match end of list
 *
 * If _node->next_ is not __NULL__, call zz_node_error() and return -1.
 *
 * @memberof zz_node
 * @param node a zz_node
 * @return 0 on success, -1 on failure
 */
int zz_match_end(struct zz_node * node);
/**
 * Print error from node
 *
 * Shorthand for node->tree->error(node, msg)
 *
 * @memberof zz_node
 * @param node a zz_node
 * @param msg error messabe to be printed
 */
void zz_node_error(struct zz_node *node, const char *msg);
/**
 * Print error message
 *
 * Prints an error message including the file name _file_ and line _line_, then
 * prints the offending line and a caret pointing at the offending column.
 *
 * @ingroup Zebu
 * @param msg error messabe to be printed
 * @param file path to the file with the error
 * @param line offending line
 * @param column offending column
 */
void zz_error(const char *msg, const char *file, size_t line, size_t column);

#endif
