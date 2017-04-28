
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
 * This implementation lacks specialized lookup routines: lookup is provided by
 * the insert method--attempting to insert an already existing string will
 * return the already allocated data.
 */
struct zz_dict {
	/* Left and right children */
	struct zz_dict *left, *right;
	/* Level of this node */
	size_t level;
	/* Reference counter */
	size_t ref_count;
	/* Data attached to this node */
	char *data;
};

int zz_dict_lookup(struct zz_dict *t, const char *data, const char **rval);

struct zz_dict *zz_dict_insert(struct zz_dict *t, const char *data, const char **rval);

struct zz_dict *zz_dict_delete(struct zz_dict *t, const char *data);

void zz_dict_destroy(struct zz_dict *t);

#endif            // ZEBU_DICT_H_
