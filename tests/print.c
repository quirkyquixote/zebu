
#include <assert.h>

#include "../src/zebu.h"

static const char *TOK_FOO = "foo";
static const char *TOK_BAR = "bar";
static const char *TOK_BAZ = "baz";

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *root;
	struct zz_node *node;

	zz_tree_init(&tree, sizeof(struct zz_node));

	root = zz_null(&tree, TOK_FOO);
	assert(root != NULL);

	node = zz_null(&tree, TOK_BAR);
	zz_append_child(root, node);
	node = zz_int(&tree, TOK_BAZ, -314);
	zz_append_child(root, node);
	node = zz_uint(&tree, TOK_BAR, 314);
	zz_append_child(root, node);
	node = zz_double(&tree, TOK_BAZ, 3.14);
	zz_append_child(root, node);
	node = zz_string(&tree, TOK_BAR, "314");
	zz_append_child(root, node);
	node = zz_pointer(&tree, TOK_BAZ, NULL);
	zz_append_child(root, node);

	zz_print(root, stdout);
	printf("\n");
	exit(EXIT_SUCCESS);
}
