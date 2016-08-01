
#include <assert.h>

#include "../zebu.h"

struct node_with_location {
	struct zz_node node;
	int location;
};

static void set_location(void *ptr, int location)
{
	struct node_with_location *node = ptr;
	node->location = location;
}

static int get_location(void *ptr)
{
	struct node_with_location *node = ptr;
	return node->location;
}

static const char *names[] = { "foo", "bar", "baz" };

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *n1, *n2, *n3, *n4, *n5, *n6;

	zz_tree_init(&tree, sizeof(struct node_with_location), names, 3);

	n1 = zz_null(&tree, 0);
	set_location(n1, 1);
	n2 = zz_int(&tree, 1, -314);
	set_location(n2, 2);
	n3 = zz_uint(&tree, 2, 314);
	set_location(n3, 3);
	n4 = zz_double(&tree, 0, 3.14);
	set_location(n4, 5);
	n5 = zz_string(&tree, 1, "314");
	set_location(n5, 8);
	n6 = zz_pointer(&tree, 2, &tree);
	set_location(n6, 13);

	assert(get_location(n1) == 1);
	assert(get_location(n2) == 2);
	assert(get_location(n3) == 3);
	assert(get_location(n4) == 5);
	assert(get_location(n5) == 8);
	assert(get_location(n6) == 13);

	exit(EXIT_SUCCESS);
}

