/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */
/**
 * @file
 * Declare zz_list struct and related functions
 */

#ifndef ZEBU_LIST_H_
#define ZEBU_LIST_H_

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>

/**
 * Doubly-linked list
 *
 * @ingroup Zebu
 */
struct zz_list {
	/** Previous node */
	struct zz_list *prev;
	/** Next node */
	struct zz_list *next;
};

/**
 * Initialize a list
 *
 * Makes the prev and next pointers point to _list_: if this is intended to be
 * a sentinel node, it represents an empty list; if it is supposed to be a
 * field inside a node with data, it represents an unlinked node.
 *
 * @memberof zz_list
 * @param [in] list a zz_list
 */
static inline void zz_list_init(struct zz_list *list)
{
	list->next = list;
	list->prev = list;
}
/**
 * Return 1 if empty
 *
 * @memberof zz_list
 * @param [in] list a zz_list
 * @return 1 if _list_ is empty, 0 otherwise
 */
static inline int zz_list_empty(struct zz_list *list)
{
	return list->next == list;
}
/**
 * Unlinks _node_ from its list
 *
 * Links together the prev and next nodes to close the gap; it doesn't relink
 * node to itself, use zz_list_init() for that.
 *
 * @memberof zz_list
 * @param [in] node node to be unlinked
 */
static inline void zz_list_unlink(struct zz_list *node)
{
	node->next->prev = node->prev;
	node->prev->next = node->next;
}
/**
 * Inserts _node_ before _next_
 *
 * If node was part of another list, it will be invalidated; you must use
 * zz_list_unlink() on it first.
 *
 * @memberof zz_list
 * @param [in] next a node in a list
 * @param [in] node node to be inserted.
 */
static inline void zz_list_insert(struct zz_list *next, struct zz_list *node)
{
	struct zz_list *prev = next->prev;
	next->prev = node;
	prev->next = node;
	node->next = next;
	node->prev = prev;
}
/**
 * Append _node_ to _list_
 *
 * @memberof zz_list
 * @param [in] list sentinel node of a list
 * @param [in] node node to be inserted
 */
static inline void zz_list_append(struct zz_list *list, struct zz_list *node)
{
	zz_list_insert(list, node);
}
/**
 * Prepend _node_ to _list_
 *
 * @memberof zz_list
 * @param [in] list sentinel node of a list
 * @param [in] node node to be inserted
 */
static inline void zz_list_prepend(struct zz_list *list, struct zz_list *node)
{
	zz_list_insert(list->next, node);
}
/**
 * Insert nodes of _other_ before _next_
 *
 * Invalidates _other_; use zz_list_init() to reset it to an empty list.
 *
 * @memberof zz_list
 * @param [in] next a node in a list
 * @param [in] other sentinel node of another list
 */
static inline void zz_list_splice(struct zz_list *next, struct zz_list *other)
{
	struct zz_list *prev = next->prev;
	prev->next = other->next;
	next->prev = other->prev;
	other->prev->next = next;
	other->next->prev = prev;
}
/**
 * Append nodes of _other_ to _list_
 *
 * Invalidates _other_; use zz_list_init() to reset it to an empty list.
 *
 * @memberof zz_list
 * @param [in] list sentinel node of a list
 * @param [in] other sentinel node of another list
 */
static inline void zz_list_append_list(struct zz_list *list, struct zz_list *other)
{
	zz_list_splice(list, other);
}
/**
 * Prepend nodes of _other_ to _list_
 *
 * Invalidates _other_; use zz_list_init() to reset it to an empty list.
 *
 * @memberof zz_list
 * @param [in] list sentinel node of a list
 * @param [in] other sentinel node of another list
 */
static inline void zz_list_prepend_list(struct zz_list *list, struct zz_list *other)
{
	zz_list_splice(list->next, other);
}
/**
 * Replace _a_ and _b_ in their respective lists
 *
 * Links the prev and next nodes of _a_ to _b_, and _b_ to _a_, except when one
 * or both of the nodes are empty, in which case the other node is linked to
 * itself.
 *
 * @memberof zz_list
 * @param [in] a node to replace b
 * @param [in] b node to replace a
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
/**
 * Iterate on a zz_list
 *
 * @memberof zz_list
 * @param [out] iter pointer to struct zz_list to be used as iterator
 * @param [in] list sentinel node of the list
 */
#define zz_list_foreach(iter, list) \
	for (iter = (list)->next; iter != (list); iter = iter->next)
/**
 * Iterate on a zz_list, backwards
 *
 * @memberof zz_list
 * @param [out] iter pointer to struct zz_list to be used as iterator
 * @param [in] list sentinel node of the list
 */
#define zz_list_reverse_foreach(iter, list) \
	for (iter = (list)->prev; iter != (list); iter = iter->prev)
/**
 * Iterate on a zz_list; allows unlinking of nodes
 *
 * @memberof zz_list
 * @param [out] iter pointer to struct zz_list to be used as iterator
 * @param [in] temp pointer to struct zz_list to be used as temporary storage
 * @param [in] list sentinel node of the list
 */
#define zz_list_foreach_safe(iter, temp, list) \
	for (iter = (list)->next; temp = iter->next, iter != (list); iter = temp)
/**
 * Iterate on a zz_list, backwards; allows unlinking of nodes
 *
 * @memberof zz_list
 * @param [out] iter pointer to struct zz_list to be used as iterator
 * @param [in] temp pointer to struct zz_list to be used as temporary storage
 * @param [in] list sentinel node of the list
 */
#define zz_list_reverse_foreach_safe(iter, temp, list) \
	for (iter = (list)->prev; temp = iter->prev, iter != (list); iter = temp)
/**
 * Get struct for iterator
 *
 * @memberof zz_list
 * @param [in] iter pointer to the zz_list
 * @param [in] type type of the struct containing the zz_list
 * @param [in] member name of the zz_list in the struct
 * @return entry for _iter_
 */
#define zz_list_entry(iter, type, member) \
	((type *)((char *)(iter) - offsetof(type, member)))
/**
 * Get struct for list head
 *
 * @memberof zz_list
 * @param [in] list the sentinel node of the list
 * @param [in] type type of the struct containing the zz_list
 * @param [in] member name of the zz_list in the struct
 * @return first entry of _list_
 */
#define zz_list_first_entry(list, type, member) \
	zz_list_entry((list)->next, type, member)
/**
 * Get struct for list tail
 *
 * @memberof zz_list
 * @param [in] list the sentinel node of the list
 * @param [in] type type of the struct containing the zz_list
 * @param [in] member name of the zz_list in the struct
 * @return last entry of _list_
 */
#define zz_list_last_entry(list, type, member) \
	zz_list_entry((list)->prev, type, member)
/**
 * Get next entry
 *
 * @memberof zz_list
 * @param [in] iter the previous entry
 * @param [in] member name of the zz_list in the struct
 * @return next entry of _iter_
 */
#define zz_list_next_entry(iter, member) \
	zz_list_entry((iter)->member.next, typeof(*iter), member)
/**
 * Get next entry
 *
 * @memberof zz_list
 * @param [in] iter the next entry
 * @param [in] member name of the zz_list in the struct
 * @return previous entry of _iter_
 */
#define zz_list_prev_entry(iter, member) \
	zz_list_entry((iter)->member.prev, typeof(*iter), member)
/**
 * Iterate on a zz_list
 *
 * @memberof zz_list
 * @param [out] iter pointer to be used as iterator
 * @param [in] list sentinel node of the list
 * @param [in] member name of the zz_list in the struct
 */
#define zz_list_foreach_entry(iter, list, member) \
	for (iter = zz_list_first_entry(list, typeof(*iter), member); \
			&iter->member != (list); \
			iter = zz_list_next_entry(iter, member))
/**
 * Iterate on a zz_list, backwards
 *
 * @memberof zz_list
 * @param [out] iter pointer to be used as iterator
 * @param [in] list sentinel node of the list
 * @param [in] member name of the zz_list in the struct
 */
#define zz_list_reverse_foreach_entry(iter, list, member) \
	for (iter = zz_list_last_entry(list, typeof(*iter), member); \
			&iter->member != (list); \
			iter = zz_list_prev_entry(iter, member))
/**
 * Iterate on a zz_list; allows unlinking of nodes
 *
 * @memberof zz_list
 * @param [out] iter pointer to be used as iterator
 * @param [in] temp pointer to be used as temporary storage
 * @param [in] list sentinel node of the list
 * @param [in] member name of the zz_list in the struct
 */
#define zz_list_foreach_entry_safe(iter, temp, list, member) \
	for (iter = zz_list_first_entry(list, typeof(*iter), member); \
			temp = zz_list_next_entry(iter, member), \
			&iter->member != (list); \
			iter = temp)
/**
 * Iterate on a zz_list, backwards; allows unlinking of nodes
 *
 * @memberof zz_list
 * @param [out] iter pointer to be used as iterator
 * @param [in] temp pointer to be used as temporary storage
 * @param [in] list sentinel node of the list
 * @param [in] member name of the zz_list in the struct
 */
#define zz_list_reverse_foreach_entry_safe(iter, temp, list, member) \
	for (iter = zz_list_last_entry(list, typeof(*iter), member); \
			temp = zz_list_prev_entry(iter, member), \
			&iter->member != (list); \
			iter = temp)
/**
 * Return element by index (slow)
 *
 * @memberof zz_list
 * @param [in] list a zz_list
 * @param [in] index index of the desired element
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
