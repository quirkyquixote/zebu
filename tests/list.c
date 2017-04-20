
#include "../src/zebu.h"

struct node {
	struct zz_list list;
	int data;
};

static inline void check(struct zz_list *list, const int *data, size_t ndata)
{
	struct zz_list *iter;
	size_t i = 0;
	zz_foreach(iter, list) {
		assert(i < ndata);
		assert(((struct node *)iter)->data == data[i]);
		++i;
	}
	assert(i == ndata);
	zz_reverse_foreach(iter, list) {
		assert(i > 0);
		--i;
		assert(((struct node *)iter)->data == data[i]);
	}
	assert(i == 0);
}

int main(int argc, char *argv[])
{
	struct zz_list l0, l1;
	struct node n0, n1, n2, n3, n4, n5, n6;

	zz_list_init(&l0);
	zz_list_init(&l1);

	zz_list_init(&n0.list);
	zz_list_init(&n1.list);
	zz_list_init(&n2.list);
	zz_list_init(&n3.list);
	zz_list_init(&n4.list);
	zz_list_init(&n5.list);
	zz_list_init(&n6.list);

	n0.data = 0;
	n1.data = 1;
	n2.data = 2;
	n3.data = 3;
	n4.data = 4;
	n5.data = 5;
	n6.data = 6;

	/* () */
	check(&l0, NULL, 0);

	/* (0) */
	zz_append(&l0, &n0.list);
	{
	static const int golden[] = { 0 };
	check(&l0, golden, sizeof(golden) / sizeof(*golden));
	}

	/* (0, 1) */
	zz_append(&l0, &n1.list);
	{
	static const int golden[] = { 0, 1 };
	check(&l0, golden, sizeof(golden) / sizeof(*golden));
	}

	/* (0, 1, 2) */
	zz_append(&l0, &n2.list);
	{
	static const int golden[] = { 0, 1, 2 };
	check(&l0, golden, sizeof(golden) / sizeof(*golden));
	}

	/* (0, 1, 3, 4, 2) */
	zz_append(&l1, &n3.list);
	zz_append(&l1, &n4.list);
	zz_splice(&n2.list, &l1);
	{
	static const int golden[] = { 0, 1, 3, 4, 2 };
	check(&l0, golden, sizeof(golden) / sizeof(*golden));
	}

	/* (0, 3, 4, 2) */
	zz_unlink(&n1.list);
	{
	static const int golden[] = { 0, 3, 4, 2 };
	check(&l0, golden, sizeof(golden) / sizeof(*golden));
	}

	/* (0, 3, 4, 2, 5) */
	zz_append(&l0, &n5.list);
	{
	static const int golden[] = { 0, 3, 4, 2, 5 };
	check(&l0, golden, sizeof(golden) / sizeof(*golden));
	}

	/* (6, 0, 3, 4, 2, 5) */
	zz_prepend(&l0, &n6.list);
	{
	static const int golden[] = { 6, 0, 3, 4, 2, 5 };
	check(&l0, golden, sizeof(golden) / sizeof(*golden));
	}

	exit(EXIT_SUCCESS);
}
