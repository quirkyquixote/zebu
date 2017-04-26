

#include <assert.h>
#include <string.h>

#include "../src/zebu.h"

static const char *TOK_FOO = "foo";
static const char *TOK_BAR = "bar";
static const char *TOK_BAZ = "baz";

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *n1, *n2, *n3, *n4;

	zz_tree_init(&tree, sizeof(struct zz_node));

	n1 = zz_node(&tree, TOK_FOO, zz_string(&tree, "foo"));
	n2 = zz_node(&tree, TOK_FOO, zz_string(&tree, "bar"));
	n3 = zz_node(&tree, TOK_FOO, zz_string(&tree, "foobar"));
	n4 = zz_node(&tree, TOK_FOO, zz_string(&tree, "foo"));

	assert(strcmp(zz_get_string(n1), "foo") == 0);
	assert(strcmp(zz_get_string(n2), "bar") == 0);
	assert(strcmp(zz_get_string(n3), "foobar") == 0);
	assert(strcmp(zz_get_string(n4), "foo") == 0);

	assert(strcmp(zz_get_string(n1), zz_get_string(n4)) == 0);

	exit(EXIT_SUCCESS);
}
