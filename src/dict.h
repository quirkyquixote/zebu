/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */
/**
 * @file
 * Declare zz_dict struct and related functions
 */

#ifndef ZEBU_DICT_H_
#define ZEBU_DICT_H_

#include <stdlib.h>

/**
 * AA Tree to provide a dictionary of strings allocated by an AST.
 *
 * An AA Tree is a simplified form of red-black tree that implements a balanced
 * binary tree. We use a variation of it where nodes are reference counted to
 * keep an index of all strings belonging to an AST.
 *
 * @ingroup Zebu
 */
struct zz_dict {
	/** Left and right children */
	struct zz_dict *left,
	/** Left and right children */
		       *right;
	/** Level of this node */
	size_t level;
	/** Reference counter */
	size_t ref_count;
	/** Data attached to this node */
	char *data;
};

/**
 * Look up string
 *
 * @memberof zz_dict
 * @param [in] t pointer to root node
 * @param [in] data string to be searched
 * @param [out] rval buffer for the original string
 * @return 1 if the string exists in the tree, 0 otherwise
 */
int zz_dict_lookup(struct zz_dict *t, const char *data, const char **rval);
/**
 * Insert string in tree
 *
 * If _data_ does not exist in the tree, a new node will be created, and a
 * copy of it will be stored in it, and passed back through the _rval_ pointer.
 *
 * If it already exists, its reference counter will be incremented by one, and
 * the original stringt will be returned through _rval_.
 *
 * @memberof zz_dict
 * @param [in] t pointer to the root node
 * @param [in] data string to be inserted
 * @param [out] rval buffer for the new string
 * @return the new root node of the tree
 */
struct zz_dict *zz_dict_insert(struct zz_dict *t, const char *data, const char **rval);
/**
 * Delete string from tree
 *
 * If _data_ exists in the tree, its reference counter will be decremented by
 * one; if it reaches zero, the node holding it will be removed.
 *
 * @memberof zz_dict
 * @param [in] t pointer to the root node
 * @param [in] data string to be removed
 * @return the new root node of the tree
 */
struct zz_dict *zz_dict_delete(struct zz_dict *t, const char *data);
/**
 * Destroy the tree
 *
 * @memberof zz_dict
 * @param [in] t pointer to the root node
 */
void zz_dict_destroy(struct zz_dict *t);

#endif            // ZEBU_DICT_H_
