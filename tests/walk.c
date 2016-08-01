
#include <assert.h>

#include "../zebu.h"

static const char *names[] = { "foo", "bar", "baz" };

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *root;
	struct zz_node *node;

	zz_tree_init(&tree, sizeof(struct zz_node), names, 3);

	assert((root = zz_null(&tree, 0)) != NULL);

	zz_append(root, zz_null(&tree, 0));
	zz_append(root, zz_int(&tree, 1, -314));
	zz_append(root, zz_uint(&tree, 2, 314));
	zz_append(root, zz_double(&tree, 0, 3.14));
	zz_append(root, zz_string(&tree, 1, "314"));
	zz_append(root, zz_pointer(&tree, 2, &tree));

	zz_match(root, 0);
	node = zz_children(root);
	zz_match(node, 0);
	node = zz_next(node);
	zz_match(node, 1);
	node = zz_next(node);
	zz_match(node, 2);
	node = zz_next(node);
	zz_match(node, 0);
	node = zz_next(node);
	zz_match(node, 1);
	node = zz_next(node);
	zz_match(node, 2);
	node = zz_next(node);
	zz_match_end(node);
	exit(EXIT_SUCCESS);
}
