
#include <assert.h>

#include "../zebu.h"

static const char *names[] = { "foo", "bar", "baz" };

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *root;
	struct zz_node *node;

	zz_tree_init(&tree, sizeof(struct zz_node), names, 3);

	root = zz_null(&tree, 0);
	assert(root != NULL);

	node = zz_null(&tree, 1);
	zz_append_child(root, node);
	node = zz_int(&tree, 2, -314);
	zz_append_child(root, node);
	node = zz_uint(&tree, 1, 314);
	zz_append_child(root, node);
	node = zz_double(&tree, 2, 3.14);
	zz_append_child(root, node);
	node = zz_string(&tree, 1, "314");
	zz_append_child(root, node);
	node = zz_pointer(&tree, 2, &tree);
	zz_append_child(root, node);

	zz_print_node(root, stdout);
	printf("\n");
	exit(EXIT_SUCCESS);
}
