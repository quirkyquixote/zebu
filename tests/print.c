
#include <assert.h>

#include "../zebu.h"

static const char *names[] = { "foo", "bar", "baz" };

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *root;
	struct zz_node *node;

	zz_tree_init(&tree, names, sizeof(names) / sizeof(*names));

	root = zz_null(&tree, 0);
	assert(root != NULL);

	zz_append(root, zz_null(&tree, 1));
	zz_append(root, zz_int(&tree, 2, -314));
	zz_append(root, zz_uint(&tree, 1, 314));
	zz_append(root, zz_double(&tree, 2, 3.14));
	zz_append(root, zz_string(&tree, 1, "314"));
	zz_append(root, zz_pointer(&tree, 2, &tree));

	zz_print_node(root, stdout);
	printf("\n");
	exit(EXIT_SUCCESS);
}
