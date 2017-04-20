
#ifndef ZEBU_NODE_H_
#define ZEBU_NODE_H_

#include "list.h"

/* 
 * Data types that may be held by a zz_node
 */
enum {
	ZZ_NULL,                    /*< Node with no information */
	ZZ_INT,                     /*< Node with an integer */
	ZZ_UINT,                    /*< Node with an unsigned integer */
	ZZ_DOUBLE,                  /*< Node with a floating point number */
	ZZ_STRING,                  /*< Node with a string */
	ZZ_POINTER                  /*< Node with user-managed data */
};

/*
 * Data that may be held by a node in the AST
 */
union zz_node_data {
	/* For ZZ_UINT */
	unsigned int uint_val;
	/* For ZZ_INT */
	int int_val;
	/* For ZZ_DOUBLE */
	double double_val;
	/* For ZZ_STRING */
	const char *str_val;
	/* For ZZ_POINTER */
	void *ptr_val;
};

/*
 * Node in an Abstract Syntax Tree 
 */
struct zz_node {
	/* Siblings; this must be the first field */
	struct zz_list siblings;
	/* Children */
	struct zz_list children;
	/* Token for this node */
	const char *token;
	/* Type of data held by the node */
	int type;
	/* Next in list of all nodes allocated by the tree */
	struct zz_node *next;
	/* Data that depends on the node type */
	union zz_node_data data;
};

static inline void zz_null_init(struct zz_node *node)
{
	node->type = ZZ_NULL;
}
/*
 * Set node data to integer
 *
 * @node a zz_node
 * @val new value for _node_
 */
static inline void zz_int_init(struct zz_node *node, int val)
{
	node->type = ZZ_INT;
	node->data.int_val = val;
}
/*
 * Set node data to unsigned integer
 *
 * @tree a zz_tree
 * @node a zz_node
 * @val new value for _node_
 */
static inline void zz_uint_init(struct zz_node *node, unsigned int val)
{
	node->type = ZZ_UINT;
	node->data.uint_val = val;
}
/*
 * Set node data to double
 *
 * @tree a zz_tree
 * @node a zz_node
 * @val new value for _node_
 */
static inline void zz_double_init(struct zz_node *node, double val)
{
	node->type = ZZ_DOUBLE;
	node->data.double_val = val;
}
/*
 * Set node data to string
 *
 * @tree a zz_tree
 * @node a zz_node
 * @val new value for _node_
 */
static inline void zz_string_init(struct zz_node *node, const char *val)
{
	node->type = ZZ_STRING;
	node->data.str_val = val;
}
/*
 * Set node data to pointer
 *
 * @tree a zz_tree
 * @node a zz_node
 * @val new value for _node_
 */
static inline void zz_pointer_init(struct zz_node *node, void *val)
{
	node->type = ZZ_POINTER;
	node->data.ptr_val = val;
}

/*
 * Return integer data associated with node 
 *
 * @node a zz_node
 * @return integer value associated with _node_
 */
static inline int zz_to_int(const struct zz_node *node)
{
	assert(node->type == ZZ_INT);
	return node->data.int_val;
}
/*
 * Return unsigned integer data associated with node 
 *
 * @node a zz_node
 * @return unsigned integer value associated with _node_
 */
static inline unsigned int zz_to_uint(const struct zz_node *node)
{
	assert(node->type == ZZ_UINT);
	return node->data.uint_val;
}
/*
 * Return float data associated with node 
 *
 * @node a zz_node
 * @return floating point value associated with _node_
 */
static inline double zz_to_double(const struct zz_node *node)
{
	assert(node->type == ZZ_DOUBLE);
	return node->data.double_val;
}
/*
 * Return string data associated with node 
 *
 * @node a zz_node
 * @return string value associated with _node_
 */
static inline const char *zz_to_string(const struct zz_node *node)
{
	assert(node->type == ZZ_STRING);
	return node->data.str_val;
}
/*
 * Return pointer data associated with node 
 *
 * @node a zz_node
 * @return pointer value associated with _node_
 */
static inline void *zz_to_pointer(const struct zz_node *node)
{
	assert(node->type == ZZ_POINTER);
	return node->data.ptr_val;
}
/*
 * Append child to node
 *
 * @pt parent for ch
 * @ch child for pt
 */
static inline void zz_append_child(struct zz_node *pt, struct zz_node *ch)
{
	zz_append(&pt->children, &ch->siblings);
}
/*
 * Prepend child to node
 *
 * @pt parent for ch
 * @ch child for pt
 */
static inline void zz_prepend_child(struct zz_node *pt, struct zz_node *ch)
{
	zz_prepend(&pt->children, &ch->siblings);
}
/*
 * Append children to node
 *
 * @pt parent for ch
 * @ch children for pt
 */
static inline void zz_append_children(struct zz_node *pt, struct zz_node *ch)
{
	zz_append_list(&pt->children, &ch->siblings);
}
/*
 * Prepend children to node
 *
 * @pt parent for ch
 * @ch children for pt
 */
static inline void zz_prepend_children(struct zz_node *pt, struct zz_node *ch)
{
	zz_prepend_list(&pt->children, &ch->siblings);
}

/*
 * Return next sibling
 *
 * @node a zz_node
 * @return pointer to the next sibling of _node_
 */
static inline struct zz_node *zz_next(const struct zz_node *node)
{
	return (void *)node->siblings.next;
}
/*
 * Return previous sibling
 *
 * @node a zz_node
 * @return pointer to the prev sibling of _node_
 */
static inline struct zz_node *zz_prev(const struct zz_node *node)
{
	return (void *)node->siblings.prev;
}
/*
 * Return first child 
 *
 * @node a zz_node
 * @return pointer to the first child of _node_, or __NULL__
 */
static inline struct zz_node *zz_first_child(const struct zz_node *node)
{
	return (void *)node->children.next;
}
/*
 * Return last child 
 *
 * @node a zz_node
 * @return pointer to the last child of _node_, or __NULL__
 */
static inline struct zz_node *zz_last_child(const struct zz_node *node)
{
	return (void *)node->children.prev;
}
/*
 * Iterate on the children of a zz_node
 */
#define zz_foreach_child(iter, parent) \
for ((iter) = (void *)(parent)->children.next; \
	(iter) != (void *)&(parent)->children; \
	(iter) = (void *)(iter)->siblings.next)
/*
 * Iterate on a zz_list, backwards
 */
#define zz_reverse_foreach_child(iter, parent) \
for ((iter) = (void *)(parent)->children.prev; \
	(iter) != (void *)&(parent)->children; \
	(iter) = (void *)(iter)->siblings.prev)

#endif          // ZEBU_NODE_H_
