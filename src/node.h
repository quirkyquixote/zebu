/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */

#ifndef ZEBU_NODE_H_
#define ZEBU_NODE_H_

#include "list.h"
#include "data.h"

/**
 * Node
 * ----
 *
 * Actual nodes that implement the Zebu AST.
 *
 * Nodes are always handled by pointer, and managed by a zz_tree object, that
 * functions as a factory for each tree.
 */

/*
 * Node in an AST
 */
struct zz_node {
	struct zz_list siblings;
	struct zz_list children;
	struct zz_list allocated;
	const char *token;
	struct zz_data data;
};

/**
 * Iterate on children list, forward and backwards; the safe functions tike an
 * additional argument that is used as temporary storage and allows unlinking
 * the iterator inside the loop.
 */
#define zz_foreach_child(iter, node) \
zz_list_foreach_entry(iter, &node->children, siblings)
#define zz_reverse_foreach_child(iter, node) \
zz_list_reverse_foreach_entry(iter, &node->children, siblings)
#define zz_foreach_child_safe(iter, temp, node) \
zz_list_foreach_entry_safe(iter, temp, &node->children, siblings)
#define zz_reverse_foreach_child_safe(iter, temp, node) \
zz_list_reverse_foreach_entry_safe(iter, temp, &node->children, siblings)
/**
 * Get next and previous sibling of node, or ``NULL`` if there isn't one
 */
static inline struct zz_node *zz_next_sibling(struct zz_node *p, struct zz_node *c)
{
	if (c->siblings.next == &p->children)
		return NULL;
	return zz_list_entry(c->siblings.next, struct zz_node, siblings);
}
static inline struct zz_node *zz_prev_sibling(struct zz_node *p, struct zz_node *c)
{
	if (c->siblings.prev == &p->children)
		return NULL;
	return zz_list_entry(c->siblings.prev, struct zz_node, siblings);
}
/**
 * Get first and last child node, or ``NULL`` if there isn't one
 */
static inline struct zz_node *zz_first_child(struct zz_node *n)
{
	if (n->children.next == &n->children)
		return NULL;
	return zz_list_entry(n->children.next, struct zz_node, siblings);
}
static inline struct zz_node *zz_last_child(struct zz_node *n)
{
	if (n->children.prev == &n->children)
		return NULL;
	return zz_list_entry(n->children.prev, struct zz_node, siblings);
}
/**
 * Destroy node and its children recursively
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
 * Append and prepend child to node
 */
static inline void zz_append_child(struct zz_node *p, struct zz_node *c)
{
	zz_list_append(&p->children, &c->siblings);
}
static inline void zz_prepend_child(struct zz_node *p, struct zz_node *c)
{
	zz_list_prepend(&p->children, &c->siblings);
}
/**
 * Remove node from its parent
 */
static inline void zz_unlink_child(struct zz_node *n)
{
	zz_list_unlink(&n->siblings);
}
/**
 * Cast node payload to specific type
 */
static inline int zz_get_int(struct zz_node *n)
{
	return zz_to_int(n->data);
}
static inline unsigned int zz_get_uint(struct zz_node *n)
{
	return zz_to_uint(n->data);
}
static inline double zz_get_double(struct zz_node *n)
{
	return zz_to_double(n->data);
}
static inline const char *zz_get_string(struct zz_node *n)
{
	return zz_to_string(n->data);
}
static inline void *zz_get_pointer(struct zz_node *n)
{
	return zz_to_pointer(n->data);
}
/**
 * Reset node payload to new data, destroying the old one
 */
static inline void zz_set_null(struct zz_node *n)
{
	zz_data_destroy(n->data);
	n->data = zz_null;
}
static inline void zz_set_int(struct zz_node *n, int d)
{
	zz_data_destroy(n->data);
	n->data = zz_int(d);
}
static inline void zz_set_uint(struct zz_node *n, unsigned int d)
{
	zz_data_destroy(n->data);
	n->data = zz_uint(d);
}
static inline void zz_set_double(struct zz_node *n, double d)
{
	zz_data_destroy(n->data);
	n->data = zz_double(d);
}
static inline void zz_set_string(struct zz_node *n, const char *d)
{
	zz_data_destroy(n->data);
	n->data = zz_string(d);
}
static inline void zz_set_pointer(struct zz_node *n, void *d)
{
	zz_data_destroy(n->data);
	n->data = zz_pointer(d);
}

#endif          // ZEBU_NODE_H_
