
#include <assert.h>

#include "../src/zebu.h"

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

static const char *TOK_FOO = "foo";
static const char *TOK_BAR = "bar";
static const char *TOK_BAZ = "baz";

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *n1, *n2, *n3, *n4, *n5, *n6;

	zz_tree_init(&tree, sizeof(struct node_with_location));

	n1 = zz_node(&tree, TOK_FOO, zz_null);
	set_location(n1, 1);
	n2 = zz_node(&tree, TOK_BAR, zz_int(-314));
	set_location(n2, 2);
	n3 = zz_node(&tree, TOK_BAZ, zz_uint(314));
	set_location(n3, 3);
	n4 = zz_node(&tree, TOK_FOO, zz_double(0.5));
	set_location(n4, 5);
	n5 = zz_node(&tree, TOK_BAR, zz_string("314"));
	set_location(n5, 8);
	n6 = zz_node(&tree, TOK_BAZ, zz_pointer(&tree));
	set_location(n6, 13);

	assert(get_location(n1) == 1);
	assert(get_location(n2) == 2);
	assert(get_location(n3) == 3);
	assert(get_location(n4) == 5);
	assert(get_location(n5) == 8);
	assert(get_location(n6) == 13);

	exit(EXIT_SUCCESS);
}

