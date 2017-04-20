
#ifndef ZEBU_DICT_H_
#define ZEBU_DICT_H_

#include <stdlib.h>

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

struct zz_dict *zz_dict_insert(struct zz_dict *t, const char *data, const char **rval);

void zz_dict_destroy(struct zz_dict *t);

#endif            // ZEBU_DICT_H_
