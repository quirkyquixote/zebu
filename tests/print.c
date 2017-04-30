
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

	root = zz_node(&tree, TOK_FOO, zz_null);
	assert(root != NULL);

	node = zz_node(&tree, TOK_BAR, zz_null);
	zz_append_child(root, node);
	node = zz_node(&tree, TOK_BAZ, zz_int(-314));
	zz_append_child(root, node);
	node = zz_node(&tree, TOK_BAR, zz_uint(314));
	zz_append_child(root, node);
	node = zz_node(&tree, TOK_BAZ, zz_double(0.5));
	zz_append_child(root, node);
	node = zz_node(&tree, TOK_BAR, zz_string("314"));
	zz_append_child(root, node);
	node = zz_node(&tree, TOK_BAZ, zz_pointer(NULL));
	zz_append_child(root, node);

	zz_print(root, stdout);
	printf("\n");
	exit(EXIT_SUCCESS);
}
