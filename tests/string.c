

#include <assert.h>
#include <string.h>

#include "../zebu.h"

static const char *TOK_FOO = "foo";
static const char *TOK_BAR = "bar";
static const char *TOK_BAZ = "baz";

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *n1, *n2, *n3, *n4;

	zz_tree_init(&tree, sizeof(struct zz_node));

	n1 = zz_string(&tree, TOK_FOO, "foo");
	n2 = zz_string(&tree, TOK_FOO, "bar");
	n3 = zz_string(&tree, TOK_FOO, "foobar");
	n4 = zz_string(&tree, TOK_FOO, "foo");

	assert(strcmp(zz_to_string(n1), "foo") == 0);
	assert(strcmp(zz_to_string(n2), "bar") == 0);
	assert(strcmp(zz_to_string(n3), "foobar") == 0);
	assert(strcmp(zz_to_string(n4), "foo") == 0);

	assert(zz_to_string(n1) == zz_to_string(n4));

	exit(EXIT_SUCCESS);
}
