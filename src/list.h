
#ifndef ZEBU_LIST_H_
#define ZEBU_LIST_H_

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>

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
 * @list list a zz_list
 */
static inline void zz_list_init(struct zz_list *list)
{
	list->next = list;
	list->prev = list;
}
/*
 * Return 1 if empty
 *
 * @list list a zz_list
 */
static inline int zz_list_empty(struct zz_list *list)
{
	return list->next == list;
}
/*
 * Unlinks a node from a list
 *
 * Links together the prev and next nodes to close the gap; it doesn't relink
 * node to itself, use zz_list_init() for that.
 *
 * @node node to unlink
 */
static inline void zz_list_unlink(struct zz_list *node)
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
static inline void zz_list_insert(struct zz_list *next, struct zz_list *node)
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
static inline void zz_list_append(struct zz_list *list, struct zz_list *node)
{
	zz_list_insert(list, node);
}
/*
 * Prepend a node to a list
 *
 * @list sentinel node of a list
 * @node node to be inserted
 */
static inline void zz_list_prepend(struct zz_list *list, struct zz_list *node)
{
	zz_list_insert(list->next, node);
}
/*
 * Insert nodes before prev
 *
 * @prev a node in a list
 * @list nodes sentinel node of a list
 */
static inline void zz_list_splice(struct zz_list *next, struct zz_list *list)
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
static inline void zz_list_append_list(struct zz_list *list, struct zz_list *nodes)
{
	zz_list_splice(list, nodes);
}
/*
 * Prepend nodes to a list
 *
 * @list sentinel node of a list
 * @nodes nodes to be inserted
 */
static inline void zz_list_prepend_list(struct zz_list *list, struct zz_list *nodes)
{
	zz_list_splice(list->next, nodes);
}
/*
 * Replace a and b in their respective lists
 *
 * Links the prev and next nodes of a to b, and b to a, except when one or both
 * of the nodes are empty, in which case the other node is linked to itself.
 *
 * @a node to replace b
 * @b node to replace a
 */
static inline void zz_list_swap(struct zz_list *a, struct zz_list *b)
{
	struct zz_list *a_prev = a->prev;
	struct zz_list *a_next = a->next;
	struct zz_list *b_prev = b->prev;
	struct zz_list *b_next = b->next;
	if (a_prev != a) {
		a_prev->next = b;
		a_next->prev = b;
		b->next = a_next;
		b->prev = a_prev;
	} else {
		zz_list_init(b);
	}
	if (b_prev != b) {
		b_prev->next = a;
		b_next->prev = a;
		a->next = b_next;
		a->prev = b_prev;
	} else {
		zz_list_init(a);
	}
}
/*
 * Iterate on a zz_list
 */
#define zz_list_foreach(iter, list) \
	for (iter = (list)->next; iter != (list); iter = iter->next)
/*
 * Iterate on a zz_list, backwards
 */
#define zz_list_reverse_foreach(iter, list) \
	for (iter = (list)->prev; iter != (list); iter = iter->prev)
/*
 * Iterate on a zz_list; allows unlinking of nodes
 */
#define zz_list_foreach_safe(iter, next, list) \
	for (iter = (list)->next; next = iter->next, iter != (list); iter = next)
/*
 * Iterate on a zz_list, backwards; allows unlinking of nodes
 */
#define zz_list_reverse_foreach_safe(iter, next, list) \
	for (iter = (list)->prev; prev = iter->prev, iter != (list); iter = prev)
/*
 * Get struct for iterator
 */
#define zz_list_entry(iter, type, member) \
	((type *)((char *)(iter) - offsetof(type, member)))
/*
 * Get struct for list head
 */
#define zz_list_first_entry(list, type, member) \
	zz_list_entry((list)->next, type, member)
/*
 * Get struct for list tail
 */
#define zz_list_last_entry(list, type, member) \
	zz_list_entry((list)->prev, type, member)
/*
 * Get next entry
 */
#define zz_list_next_entry(iter, member) \
	zz_list_entry((iter)->member.next, typeof(*iter), member)
/*
 * Get next entry
 */
#define zz_list_prev_entry(iter, member) \
	zz_list_entry((iter)->member.prev, typeof(*iter), member)
/*
 * Iterate on a zz_list
 */
#define zz_list_foreach_entry(iter, list, member) \
	for (iter = zz_list_first_entry(list, typeof(*iter), member); \
			&iter->member != (list); \
			iter = zz_list_next_entry(iter, member))
/*
 * Iterate on a zz_list, backwards
 */
#define zz_list_reverse_foreach_entry(iter, list, member) \
	for (iter = zz_list_last_entry(list, typeof(*iter), member); \
			&iter->member != (list); \
			iter = zz_list_prev_entry(iter, member))
/*
 * Iterate on a zz_list; allows unlinking of nodes
 */
#define zz_list_foreach_entry_safe(iter, next, list, member) \
	for (iter = zz_list_first_entry(list, typeof(*iter), member); \
			next = zz_list_next_entry(iter, member), \
			&iter->member != (list); \
			iter = next)
/*
 * Iterate on a zz_list, backwards; allows unlinking of nodes
 */
#define zz_list_reverse_foreach_entry_safe(iter, next, list, member) \
	for (iter = zz_list_last_entry(list, typeof(*iter), member); \
			prev = zz_list_prev_entry(iter, member), \
			&iter->member != (list); \
			iter = prev)

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
	zz_list_foreach(iter, list) {
		if (index == 0)
			return iter;
		--index;
	}
	return NULL;
}

#endif       // ZEBU_LIST_H_
