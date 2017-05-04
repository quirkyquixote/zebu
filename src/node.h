/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */
/**
 * @file
 * Declare zz_node struct and related functions
 */

#ifndef ZEBU_NODE_H_
#define ZEBU_NODE_H_

#include "list.h"
#include "data.h"

/**
 * Node in an AST
 *
 * Note that to create new nodes, a zz_tree struct is required as a factory.
 *
 * @ingroup Zebu
 */
struct zz_node {
	/** Siblings */
	struct zz_list siblings;
	/** Children */
	struct zz_list children;
	/** All nodes in tree */
	struct zz_list allocated;
	/** Token for this node */
	const char *token;
	/** Payload */
	struct zz_data data;
};

/**
 * Iterate on children list
 *
 * @memberof zz_node
 * @param [out] iter pointer to struct zz_node to be used as iterator
 * @param [in] node node over which children we want to iterate
 */
#define zz_foreach_child(iter, node) \
zz_list_foreach_entry(iter, &node->children, siblings)
/**
 * Iterate on children list, backwards
 *
 * @memberof zz_node
 * @param [out] iter pointer to struct zz_node to be used as iterator
 * @param [in] node node over which children we want to iterate
 */
#define zz_reverse_foreach_child(iter, node) \
zz_list_reverse_foreach_entry(iter, &node->children, siblings)
/**
 * Iterate on children list; allows removal
 *
 * @memberof zz_node
 * @param [out] iter pointer to struct zz_node to be used as iterator
 * @param [in] temp pointer to struct zz_node to be used as temporary storage
 * @param [in] node node over which children we want to iterate
 */
#define zz_foreach_child_safe(iter, temp, node) \
zz_list_foreach_entry_safe(iter, temp, &node->children, siblings)
/**
 * Iterate on children list, backwards; allows removal
 *
 * @memberof zz_node
 * @param [out] iter pointer to struct zz_node to be used as iterator
 * @param [in] temp pointer to struct zz_node to be used as temporary storage
 * @param [in] node node over which children we want to iterate
 */
#define zz_reverse_foreach_child_safe(iter, temp, node) \
zz_list_reverse_foreach_entry_safe(iter, temp, &node->children, siblings)
/**
 * Get next sibling of node, or __NULL__
 *
 * @memberof zz_node
 * @param [in] p parent of _c_
 * @param [in] c previous node
 * @return next child of _p_, or __NULL__
 */
static inline struct zz_node *zz_next_sibling(struct zz_node *p, struct zz_node *c)
{
	if (c->siblings.next == &p->children)
		return NULL;
	return zz_list_entry(c->siblings.next, struct zz_node, siblings);
}
/**
 * Get previous sibling of node, or __NULL__
 *
 * @memberof zz_node
 * @param [in] p parent of _c_
 * @param [in] c next node
 * @return next child of _p_, or __NULL__
 */
static inline struct zz_node *zz_prev_sibling(struct zz_node *p, struct zz_node *c)
{
	if (c->siblings.prev == &p->children)
		return NULL;
	return zz_list_entry(c->siblings.prev, struct zz_node, siblings);
}
/**
 * Get first child node, or __NULL__
 *
 * @memberof zz_node
 * @param [in] n parent node
 * @return first child of _n_, or __NULL__
 */
static inline struct zz_node *zz_first_child(struct zz_node *n)
{
	if (n->children.next == &n->children)
		return NULL;
	return zz_list_entry(n->children.next, struct zz_node, siblings);
}
/**
 * Get last child node, or __NULL__
 *
 * @memberof zz_node
 * @param [in] n parent node
 * @return last child of _n_, or __NULL__
 */
static inline struct zz_node *zz_last_child(struct zz_node *n)
{
	if (n->children.prev == &n->children)
		return NULL;
	return zz_list_entry(n->children.prev, struct zz_node, siblings);
}
/**
 * Destroy node and its children recursively
 *
 * @memberof zz_node
 * @param [in] n node to be destroyed
 */
static inline void zz_destroy(struct zz_node *n)
{
	struct zz_node *i, *x;
	zz_foreach_child_safe(i, x, n)
		zz_destroy(i);
	zz_list_unlink(&n->allocated);
	zz_data_destroy(n->data);
	free(n);
}
/**
 * Append child to node
 *
 * @memberof zz_node
 * @param [in] p parent node
 * @param [in] c child to be appended
 */
static inline void zz_append_child(struct zz_node *p, struct zz_node *c)
{
	zz_list_append(&p->children, &c->siblings);
}
/**
 * Prepend child to node
 *
 * @memberof zz_node
 * @param [in] p parent node
 * @param [in] c child to be prepended
 */
static inline void zz_prepend_child(struct zz_node *p, struct zz_node *c)
{
	zz_list_prepend(&p->children, &c->siblings);
}
/**
 * Remove node from its parent
 *
 * @memberof zz_node
 * @param [in] n node to be removed
 */
static inline void zz_unlink_child(struct zz_node *n)
{
	zz_list_unlink(&n->siblings);
}
/**
 * Cast node payload to int
 *
 * @memberof zz_node
 * @param [in] n a zz_node
 * @returns _n_'s payload, cast to int
 */
static inline int zz_get_int(struct zz_node *n)
{
	return zz_to_int(n->data);
}
/**
 * Cast node payload to unsigned int
 *
 * @memberof zz_node
 * @param [in] n a zz_node
 * @returns _n_'s payload, cast to unsigned int
 */
static inline unsigned int zz_get_uint(struct zz_node *n)
{
	return zz_to_uint(n->data);
}
/**
 * Cast node payload to double
 *
 * @memberof zz_node
 * @param [in] n a zz_node
 * @returns _n_'s payload, cast to double
 */
static inline double zz_get_double(struct zz_node *n)
{
	return zz_to_double(n->data);
}
/**
 * Cast node payload to string
 *
 * @memberof zz_node
 * @param [in] n a zz_node
 * @returns _n_'s payload, cast to string
 */
static inline const char *zz_get_string(struct zz_node *n)
{
	return zz_to_string(n->data);
}
/**
 * Cast node payload to pointer
 *
 * @memberof zz_node
 * @param [in] n a zz_node
 * @returns _n_'s payload, cast to pointer
 */
static inline void *zz_get_pointer(struct zz_node *n)
{
	return zz_to_pointer(n->data);
}
/**
 * Reset node payload to null, destroying the old one
 *
 * @memberof zz_node
 * @param [in] n a zz_node
 */
static inline void zz_set_null(struct zz_node *n)
{
	zz_data_destroy(n->data);
	n->data = zz_null;
}
/**
 * Reset node payload to integer, destroying the old one
 *
 * @memberof zz_node
 * @param [in] n a zz_node
 * @param [in] d new payload for _n_
 */
static inline void zz_set_int(struct zz_node *n, int d)
{
	zz_data_destroy(n->data);
	n->data = zz_int(d);
}
/**
 * Reset node payload to unsigned integer, destroying the old one
 *
 * @memberof zz_node
 * @param [in] n a zz_node
 * @param [in] d new payload for _n_
 */
static inline void zz_set_uint(struct zz_node *n, unsigned int d)
{
	zz_data_destroy(n->data);
	n->data = zz_uint(d);
}

/**
 * Reset node payload to double, destroying the old one
 *
 * @memberof zz_node
 * @param [in] n a zz_node
 * @param [in] d new payload for _n_
 */
static inline void zz_set_double(struct zz_node *n, double d)
{
	zz_data_destroy(n->data);
	n->data = zz_double(d);
}
/**
 * Reset node payload to string, destroying the old one
 *
 * @memberof zz_node
 * @param [in] n a zz_node
 * @param [in] d new payload for _n_
 */
static inline void zz_set_string(struct zz_node *n, const char *d)
{
	zz_data_destroy(n->data);
	n->data = zz_string(d);
}
/**
 * Reset node payload to pointer, destroying the old one
 *
 * @memberof zz_node
 * @param [in] n a zz_node
 * @param [in] d new payload for _n_
 */
static inline void zz_set_pointer(struct zz_node *n, void *d)
{
	zz_data_destroy(n->data);
	n->data = zz_pointer(d);
}

#endif          // ZEBU_NODE_H_
