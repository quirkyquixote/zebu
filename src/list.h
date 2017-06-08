/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */

#ifndef ZEBU_LIST_H_
#define ZEBU_LIST_H_

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
#include <type_traits>
#define zz_typeof(x) std::remove_reference<decltype((x))>::type
extern "C" {
#else
#define zz_typeof(x) typeof(x)
#endif

/**
 * List
 * ----
 *
 * Doubly-linked list, highly inspired by the Linux kernel implementation.
 */

/**
 * Doubly-linked list
 */
struct zz_list {
	struct zz_list *prev, *next;
};

/**
 * Initialize. Makes the ``prev`` and ``next`` pointers point to ``list``: if
 * this is intended to be a sentinel node, it represents an empty list; if it
 * is supposed to be a field inside a node with data, it represents an unlinked
 * node.
 */
static inline void zz_list_init(struct zz_list *list)
{
	list->next = list;
	list->prev = list;
}
/**
 * Return ``1`` if empty; ``0`` otherwise
 */
static inline int zz_list_empty(struct zz_list *list)
{
	return list->next == list;
}
/**
 * Unlinks ``node`` from its list by linking together the ``prev`` and ``next``
 * nodes to close the gap; it doesn't relink ``node`` to itself, use
 * zz_list_init() for that.
 */
static inline void zz_list_unlink(struct zz_list *node)
{
	node->next->prev = node->prev;
	node->prev->next = node->next;
}
/**
 * Inserts ``node`` before ``next``. If ``node`` was part of another list, it
 * will be invalidated; you must use zz_list_unlink() on it first.
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
 * Append and prepend ``node`` to ``list``
 */
static inline void zz_list_append(struct zz_list *list, struct zz_list *node)
{
	zz_list_insert(list, node);
}
static inline void zz_list_prepend(struct zz_list *list, struct zz_list *node)
{
	zz_list_insert(list->next, node);
}
/**
 * Insert nodes of ``other`` before ``next``. Invalidates ``other``; use
 * zz_list_init() to reset it to an empty list.
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
 * Append and prepend nodes of ``other`` to ``list``. Invalidates ``other``;
 * use zz_list_init() to reset it to an empty list.
 */
static inline void zz_list_append_list(struct zz_list *list, struct zz_list *other)
{
	zz_list_splice(list, other);
}
static inline void zz_list_prepend_list(struct zz_list *list, struct zz_list *other)
{
	zz_list_splice(list->next, other);
}
/**
 * Replace ``a`` and ``b`` in their respective lists. Links the ``prev`` and
 * ``next`` nodes of ``a`` to ``b``, and ``b`` to ``a``, except when one
 * or both of the nodes are empty, in which case the other node is linked to
 * itself.
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
 * Iterate on a zz_list, in different directions; the safe functions tike an
 * additional argument that is used as temporary storage and allows unlinking
 * the iterator inside the loop.
 */
#define zz_list_foreach(iter, list) \
	for (iter = (list)->next; iter != (list); iter = iter->next)
#define zz_list_reverse_foreach(iter, list) \
	for (iter = (list)->prev; iter != (list); iter = iter->prev)
#define zz_list_foreach_safe(iter, temp, list) \
	for (iter = (list)->next; temp = iter->next, iter != (list); iter = temp)
#define zz_list_reverse_foreach_safe(iter, temp, list) \
	for (iter = (list)->prev; temp = iter->prev, iter != (list); iter = temp)
/**
 * Get struct for iterator
 */
#define zz_list_entry(iter, type, member) \
	((type *)((char *)(iter) - offsetof(type, member)))
/**
 * Get struct for list head and tail
 */
#define zz_list_first_entry(list, type, member) \
	zz_list_entry((list)->next, type, member)
#define zz_list_last_entry(list, type, member) \
	zz_list_entry((list)->prev, type, member)
/**
 * Get next and prev entry
 */
#define zz_list_next_entry(iter, member) \
	zz_list_entry((iter)->member.next, zz_typeof(*iter), member)
#define zz_list_prev_entry(iter, member) \
	zz_list_entry((iter)->member.prev, zz_typeof(*iter), member)
/**
 * Iterate on a zz_list, in different directions; the safe functions tike an
 * additional argument that is used as temporary storage and allows unlinking
 * the iterator inside the loop.
 */
#define zz_list_foreach_entry(iter, list, member) \
	for (iter = zz_list_first_entry(list, zz_typeof(*iter), member); \
			&iter->member != (list); \
			iter = zz_list_next_entry(iter, member))
#define zz_list_reverse_foreach_entry(iter, list, member) \
	for (iter = zz_list_last_entry(list, zz_typeof(*iter), member); \
			&iter->member != (list); \
			iter = zz_list_prev_entry(iter, member))
#define zz_list_foreach_entry_safe(iter, temp, list, member) \
	for (iter = zz_list_first_entry(list, zz_typeof(*iter), member); \
			temp = zz_list_next_entry(iter, member), \
			&iter->member != (list); \
			iter = temp)
#define zz_list_reverse_foreach_entry_safe(iter, temp, list, member) \
	for (iter = zz_list_last_entry(list, zz_typeof(*iter), member); \
			temp = zz_list_prev_entry(iter, member), \
			&iter->member != (list); \
			iter = temp)
/**
 * Return element by index (slow)
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

#ifdef __cplusplus
}
#endif

#endif       // ZEBU_LIST_H_
