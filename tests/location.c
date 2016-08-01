
#include <assert.h>

#include "../zebu.h"

static const char *names[] = { "foo", "bar", "baz" };

void gen_loc(struct zz_location *loc, void *user)
{
	int *counter = user;
	loc->line = *counter;
	loc->column = *counter;
	++*counter;
}

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	int counter;
	struct zz_node *n1, *n2, *n3, *n4, *n5, *n6;

	counter = 0;
	zz_tree_init(&tree, names, sizeof(names) / sizeof(*names));
	tree.gen_loc = gen_loc;
	tree.gen_loc_data = &counter;

	n1 = zz_null(&tree, 0);
	n2 = zz_int(&tree, 1, -314);
	n3 = zz_uint(&tree, 2, 314);
	n4 = zz_double(&tree, 0, 3.14);
	n5 = zz_string(&tree, 1, "314");
	n6 = zz_pointer(&tree, 2, &tree);

	assert(n1->loc.line == 0 && n1->loc.column == 0);
	assert(n2->loc.line == 1 && n2->loc.column == 1);
	assert(n3->loc.line == 2 && n3->loc.column == 2);
	assert(n4->loc.line == 3 && n4->loc.column == 3);
	assert(n5->loc.line == 4 && n5->loc.column == 4);
	assert(n6->loc.line == 5 && n6->loc.column == 5);

	exit(EXIT_SUCCESS);
}

