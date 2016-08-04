
#include "../zebu.h"

int main(int argc, char *argv[])
{
	struct zz_list n0, n1, n2, n3, n4, n5, n6;

	zz_list_init(&n0);
	zz_list_init(&n1);
	zz_list_init(&n2);
	zz_list_init(&n3);
	zz_list_init(&n4);
	zz_list_init(&n5);
	zz_list_init(&n6);

	/* (0) */
	assert(n0.next == &n0);
	assert(n0.prev == &n0);

	/* (0, 1) */
	zz_insert(&n0, &n1);
	assert(n0.next == &n1);
	assert(n1.next == &n0);
	assert(n0.prev == &n1);
	assert(n1.prev == &n0);

	/* (0, 1, 2) */
	zz_insert(&n1, &n2);
	assert(n0.next == &n1);
	assert(n1.next == &n2);
	assert(n2.next == &n0);
	assert(n0.prev == &n2);
	assert(n1.prev == &n0);
	assert(n2.prev == &n1);

	/* (0, 1, 3, 4, 2) */
	zz_insert(&n3, &n4);
	zz_splice(&n1, &n3);
	assert(n0.next == &n1);
	assert(n1.next == &n3);
	assert(n3.next == &n4);
	assert(n4.next == &n2);
	assert(n2.next == &n0);
	assert(n0.prev == &n2);
	assert(n1.prev == &n0);
	assert(n3.prev == &n1);
	assert(n4.prev == &n3);
	assert(n2.prev == &n4);

	/* (0, 3, 4, 2) */
	zz_unlink(&n1);
	assert(n0.next == &n3);
	assert(n3.next == &n4);
	assert(n4.next == &n2);
	assert(n2.next == &n0);
	assert(n0.prev == &n2);
	assert(n3.prev == &n0);
	assert(n4.prev == &n3);
	assert(n2.prev == &n4);

	/* (0, 3, 4, 2, 5) */
	zz_append(&n0, &n5);
	assert(n0.next == &n3);
	assert(n3.next == &n4);
	assert(n4.next == &n2);
	assert(n2.next == &n5);
	assert(n5.next == &n0);
	assert(n0.prev == &n5);
	assert(n3.prev == &n0);
	assert(n4.prev == &n3);
	assert(n2.prev == &n4);
	assert(n5.prev == &n2);

	/* (6, 0, 3, 4, 2, 5) */
	zz_prepend(&n5, &n6);
	assert(n6.next == &n0);
	assert(n0.next == &n3);
	assert(n3.next == &n4);
	assert(n4.next == &n2);
	assert(n2.next == &n5);
	assert(n5.next == &n6);
	assert(n6.prev == &n5);
	assert(n0.prev == &n6);
	assert(n3.prev == &n0);
	assert(n4.prev == &n3);
	assert(n2.prev == &n4);
	assert(n5.prev == &n2);

	exit(EXIT_SUCCESS);
}
