
#ifndef ZEBU_LIST_H_
#define ZEBU_LIST_H_

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/*
 * Doubly-linked list
 */
struct zz_list {
	/* Previous node */
	struct zz_list *prev;
	/* Next node */
	struct zz_list *next;
};

/*
 * Initialize a list
 *
 * Makes the prev and next pointers point to list: if this is intended to be a
 * sentinel node, it represents an empty list; if it is supposed to be a field
 * inside a node with data, it represents an unlinked node.
 *
 * @list a zz_list
 */
static inline void zz_list_init(struct zz_list *list)
{
	list->next = list;
	list->prev = list;
}
/*
 * Unlinks a node from a list
 *
 * Links together the prev and next nodes to close the gap; it doesn't relink
 * node to itself, use zz_list_init() for that.
 *
 * @node node to unlink
 */
static inline void zz_unlink(struct zz_list *node)
{
	node->next->prev = node->prev;
	node->prev->next = node->next;
}
/*
 * Inserts node before list
 *
 * If node was part of another list, it will be invalidated; you must use
 * zz_list_unlink() on it first.
 *
 * @prev a node in a list
 * @node node to be inserted.
 */
static inline void zz_insert(struct zz_list *next, struct zz_list *node)
{
	struct zz_list *prev = next->prev;
	next->prev = node;
	prev->next = node;
	node->next = next;
	node->prev = prev;
}
/*
 * Append a node to a list
 *
 * @list sentinel node of a list
 * @node node to be inserted
 */
static inline void zz_append(struct zz_list *list, struct zz_list *node)
{
	zz_insert(list, node);
}
/*
 * Prepend a node to a list
 *
 * @list sentinel node of a list
 * @node node to be inserted
 */
static inline void zz_prepend(struct zz_list *list, struct zz_list *node)
{
	zz_insert(list->next, node);
}
/*
 * Insert nodes before prev
 *
 * @prev a node in a list
 * @list nodes sentinel node of a list
 */
static inline void zz_splice(struct zz_list *next, struct zz_list *list)
{
	struct zz_list *prev = next->prev;
	prev->next = list->next;
	next->prev = list->prev;
	list->prev->next = next;
	list->next->prev = prev;
}
/*
 * Append nodes to a list
 *
 * @list sentinel node of a list
 * @nodes nodes to be inserted
 */
static inline void zz_append_list(struct zz_list *list, struct zz_list *nodes)
{
	zz_splice(list, nodes);
}
/*
 * Prepend nodes to a list
 *
 * @list sentinel node of a list
 * @nodes nodes to be inserted
 */
static inline void zz_prepend_list(struct zz_list *list, struct zz_list *nodes)
{
	zz_splice(list->next, nodes);
}
/*
 * Replace old_node by node
 *
 * Links the prev and next nodes of old_node to node instead; it doesn't relink
 * old_node to itself, uze zz_list_init() for that; if node already belonged to
 * a list, it will be invalidaded, use zz_list_unlink() on it first.
 *
 * @old_node the node to be replaced
 * @node a node to replace old_node
 */
static inline void zz_swap(struct zz_list *old_node, struct zz_list *node)
{
	struct zz_list *prev = old_node->prev;
	struct zz_list *next = old_node->next;
	prev->next = node;
	next->prev = node;
	node->next = next;
	node->prev = prev;
}
/*
 * Iterate on a zz_list
 */
#define zz_foreach(iter, list) \
for ((iter) = (list)->next; (iter) != (list); (iter) = (iter)->next)
/*
 * Iterate on a zz_list, backwards
 */
#define zz_reverse_foreach(iter, list) \
for ((iter) = (list)->prev; (iter) != (list); (iter) = (iter)->prev)

/*
 * Return element by index (slow)
 *
 * @list a zz_list
 * @index index of the desired element
 * @return pointer to a node, or __NULL__
 */
static inline struct zz_list *zz_list_index(struct zz_list *list, size_t index)
{
	struct zz_list *iter;
	zz_foreach(iter, list) {
		if (index == 0)
			return iter;
		--index;
	}
	return NULL;
}

#endif       // ZEBU_LIST_H_
