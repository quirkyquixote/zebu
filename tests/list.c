
#include "../src/zebu.h"

struct node {
	struct zz_list list;
	int data;
};


struct zz_list *zz_list(void)
{
	struct zz_list *l = calloc(1, sizeof(*l));
	zz_list_init(l);
	return l;
}

void zz_list_destroy(struct zz_list *l)
{
	struct node *i, *n;
	zz_list_foreach_entry_safe(i, n, l, list)
		free(i);
	free(l);
}

struct node *node(int data)
{
	struct node *n = calloc(1, sizeof(*n));
	zz_list_init(&n->list);
	n->data = data;
	return n;
}

void empty_list(void)
{
	struct zz_list *head, *iter;
	int i;
	struct node *entry;

	head = zz_list();
	assert(head->next == head);
	assert(head->prev == head);
	assert(zz_list_empty(head));

	i = 0;
	zz_list_foreach(iter, head)
		++i;
	assert(i == 0);

	i = 0;
	zz_list_reverse_foreach(iter, head)
		++i;
	assert(i == 0);

	i = 0;
	zz_list_foreach_entry(entry, head, list)
		++i;
	assert(i == 0);

	i = 0;
	zz_list_reverse_foreach_entry(entry, head, list)
		++i;
	assert(i == 0);

	zz_list_destroy(head);
}

void iterate_list(void)
{
	struct zz_list *head, *iter;
	int i;
	struct node *entry;

	head = zz_list();

	for (i = 0; i < 256; ++i)
		zz_list_append(head, &node(i)->list);

	i = 0;
	zz_list_foreach(iter, head)
		++i;
	assert(i == 256);

	i = 0;
	zz_list_reverse_foreach(iter, head)
		++i;
	assert(i == 256);

	i = 0;
	zz_list_foreach_entry(entry, head, list)
		assert(entry->data == i++);
	assert(i == 256);

	i = 0;
	zz_list_reverse_foreach_entry(entry, head, list)
		assert(entry->data == 255 - i++);
	assert(i == 256);

	zz_list_destroy(head);
}

void split_list(void)
{
	struct zz_list *head, *head2, *iter;
	int i;
	struct node *entry, *next;

	head = zz_list();
	head2 = zz_list();

	for (i = 0; i < 256; ++i)
		zz_list_append(head, &node(i)->list);

	zz_list_foreach_entry_safe(entry, next, head, list) {
		if (entry->data % 2) {
			zz_list_unlink(&entry->list);
			zz_list_append(head2, &entry->list);
		}
	}

	i = 0;
	zz_list_foreach(iter, head)
		++i;
	assert(i == 128);

	i = 0;
	zz_list_reverse_foreach(iter, head)
		++i;
	assert(i == 128);

	i = 0;
	zz_list_foreach_entry(entry, head, list)
		assert(entry->data == 2 * i++);
	assert(i == 128);

	i = 0;
	zz_list_reverse_foreach_entry(entry, head, list)
		assert(entry->data == 254 - 2 * i++);
	assert(i == 128);

	i = 0;
	zz_list_foreach(iter, head2)
		++i;
	assert(i == 128);

	i = 0;
	zz_list_reverse_foreach(iter, head2)
		++i;
	assert(i == 128);

	i = 0;
	zz_list_foreach_entry(entry, head2, list)
		assert(entry->data == 2 * i++ + 1);
	assert(i == 128);

	i = 0;
	zz_list_reverse_foreach_entry(entry, head2, list)
		assert(entry->data == 255 - 2 * i++);
	assert(i == 128);

	zz_list_destroy(head);
	zz_list_destroy(head2);
}

void clear_list(void)
{
	struct zz_list *head, *iter;
	int i;
	struct node *entry, *next;

	head = zz_list();

	for (i = 0; i < 256; ++i)
		zz_list_append(head, &node(i)->list);

	zz_list_foreach_entry_safe(entry, next, head, list) {
		zz_list_unlink(&entry->list);
		free(entry);
	}

	assert(head->next == head);
	assert(head->prev == head);
	assert(zz_list_empty(head));

	i = 0;
	zz_list_foreach(iter, head)
		++i;
	assert(i == 0);

	i = 0;
	zz_list_reverse_foreach(iter, head)
		++i;
	assert(i == 0);

	i = 0;
	zz_list_foreach_entry(entry, head, list)
		++i;
	assert(i == 0);

	i = 0;
	zz_list_reverse_foreach_entry(entry, head, list)
		++i;
	assert(i == 0);

	zz_list_destroy(head);
}

void swap_list(void)
{
	struct zz_list *head, *head2, *iter, *iter2, *next, *next2;
	int i;
	struct node *entry;

	head = zz_list();
	head2 = zz_list();

	for (i = 0; i < 256; ++i) {
		zz_list_append(head, &node(i)->list);
		zz_list_append(head2, &node(-i)->list);
	}

	iter2 = head2->next;
	zz_list_foreach_safe(iter, next, head) {
		next2 = iter2->next;
		zz_list_swap(iter, iter2);
		iter2 = next2;
	}
	assert(iter2 == head2);

	i = 0;
	zz_list_foreach(iter, head)
		++i;
	assert(i == 256);

	i = 0;
	zz_list_reverse_foreach(iter, head)
		++i;
	assert(i == 256);

	i = 0;
	zz_list_foreach_entry(entry, head, list)
		assert(entry->data == -i++);
	assert(i == 256);

	i = 0;
	zz_list_reverse_foreach_entry(entry, head, list)
		assert(entry->data == i++ - 255);
	assert(i == 256);

	i = 0;
	zz_list_foreach(iter, head2)
		++i;
	assert(i == 256);

	i = 0;
	zz_list_reverse_foreach(iter, head2)
		++i;
	assert(i == 256);

	i = 0;
	zz_list_foreach_entry(entry, head2, list)
		assert(entry->data == i++);
	assert(i == 256);

	i = 0;
	zz_list_reverse_foreach_entry(entry, head2, list)
		assert(entry->data == 255 - i++);
	assert(i == 256);

	zz_list_destroy(head);
	zz_list_destroy(head2);
}

int main(int argc, char *argv[])
{
	empty_list();
	iterate_list();
	clear_list();
	split_list();
	swap_list();
	exit(EXIT_SUCCESS);
}
