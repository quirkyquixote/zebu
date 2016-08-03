

#include <assert.h>
#include <string.h>

#include "../zebu.h"

static const char *names[] = { "foo", "bar", "baz" };

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *n1, *n2, *n3, *n4;

	zz_tree_init(&tree, sizeof(struct zz_node), names, 3);

	n1 = zz_string(&tree, 0, "foo");
	n2 = zz_string(&tree, 0, "bar");
	n3 = zz_string(&tree, 0, "foobar");
	n4 = zz_string(&tree, 0, "foo");

	assert(strcmp(zz_to_string(n1), "foo") == 0);
	assert(strcmp(zz_to_string(n2), "bar") == 0);
	assert(strcmp(zz_to_string(n3), "foobar") == 0);
	assert(strcmp(zz_to_string(n4), "foo") == 0);

	assert(zz_to_string(n1) == zz_to_string(n4));

	exit(EXIT_SUCCESS);
}
