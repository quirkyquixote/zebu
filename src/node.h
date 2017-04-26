
#ifndef ZEBU_NODE_H_
#define ZEBU_NODE_H_

#include "list.h"
#include "data.h"

struct zz_node {
	/* Reference count */
	int ref_count;
	/* Siblings */
	struct zz_list siblings;
	/* Children */
	struct zz_list children;
	/* All nodes in tree */
	struct zz_list allocated;
	/* Token for this node */
	const char *token;
	/* Payload */
	struct zz_data data;
};

static inline struct zz_node *zz_ref(struct zz_node *n)
{
	++n->ref_count;
	return n;
}

static inline struct zz_node *zz_next_sibling(struct zz_node *p, struct zz_node *c)
{
	if (c->siblings.next == &p->children)
		return NULL;
	return zz_ref(zz_list_entry(c->siblings.next, struct zz_node, siblings));
}

static inline struct zz_node *zz_prev_sibling(struct zz_node *p, struct zz_node *c)
{
	if (c->siblings.prev == &p->children)
		return NULL;
	return zz_ref(zz_list_entry(c->siblings.prev, struct zz_node, siblings));
}

static inline struct zz_node *zz_first_child(struct zz_node *n)
{
	if (n->children.next == &n->children)
		return NULL;
	return zz_ref(zz_list_entry(n->children.next, struct zz_node, siblings));
}

static inline struct zz_node *zz_last_child(struct zz_node *n)
{
	if (n->children.prev == &n->children)
		return NULL;
	return zz_ref(zz_list_entry(n->children.prev, struct zz_node, siblings));
}

static inline struct zz_node *zz_unref(struct zz_node *n)
{
	if (--n->ref_count <= 0) {
		struct zz_node *i, *x;
		zz_list_foreach_entry_safe(i, x, &n->children, siblings)
			zz_unref(i);
		zz_list_unlink(&n->allocated);
		free(n);
		return NULL;
	}
	return n;
}

static inline void zz_append_child(struct zz_node *p, struct zz_node *c)
{
	zz_ref(c);
	zz_list_append(&p->children, &c->siblings);
}

static inline void zz_prepend_child(struct zz_node *p, struct zz_node *c)
{
	zz_ref(c);
	zz_list_prepend(&p->children, &c->siblings);
}

static inline void zz_unlink_child(struct zz_node *n)
{
	zz_list_unlink(&n->siblings);
	zz_unref(n);
}

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

#endif          // ZEBU_NODE_H_
