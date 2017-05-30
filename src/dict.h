/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */

#ifndef ZEBU_DICT_H_
#define ZEBU_DICT_H_

#include <stdlib.h>

/**
 * Dict
 * ----
 *
 * Dictionary of strings allocated by Zebu ASTs; implemented as an AA tree.
 *
 * An AA Tree is a simplified form of red-black tree that implements a balanced
 * binary tree. We use a variation of it where nodes are reference counted to
 * keep an index of all strings belonging to an AST.
 */

/**
 * Reference-counted node in an AA tree
 */
struct zz_dict {
	struct zz_dict *left, *right;
	size_t level;
	size_t ref_count;
	char *data;
};

/**
 * Look up string. Returns 1 if a string equal to ``data`` exists in the
 * dictionary and 0 otherwise; if it exists, the actual string is returned in
 * ``rval``.
 */
int zz_dict_lookup(struct zz_dict *t, const char *data, const char **rval);
/**
 * Insert string in tree. If ``data`` does not exist in the tree, a new node
 * will be created, and a copy of it will be stored in it, and passed back
 * through the ``rval`` pointer; if it already exists, its reference counter
 * will be incremented by one, and the original stringt will be returned
 * through ``rval``.
 */
struct zz_dict *zz_dict_insert(struct zz_dict *t, const char *data, const char **rval);
/**
 * Delete string from tree. If ``data`` exists in the tree, its reference
 * counter will be decremented by one; if it reaches zero, the node holding it
 * will be removed.
 */
struct zz_dict *zz_dict_delete(struct zz_dict *t, const char *data);
/**
 * Destroy the tree
 */
void zz_dict_destroy(struct zz_dict *t);

#endif            // ZEBU_DICT_H_
