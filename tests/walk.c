
#include <assert.h>

#include "../zebu.h"

static const char *names[] = { "foo", "bar", "baz" };

static void error(void *node, const char *msg)
{
	fprintf(stderr, "error: %s\n", msg);
}

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *root;
	struct zz_node *node;

	zz_tree_init(&tree, sizeof(struct zz_node), names, 3);
	tree.error = error;

	assert((root = zz_null(&tree, 0)) != NULL);

	zz_append_child(root, zz_null(&tree, 0));
	zz_append_child(root, zz_int(&tree, 1, -314));
	zz_append_child(root, zz_uint(&tree, 2, 314));
	zz_append_child(root, zz_double(&tree, 0, 3.14));
	zz_append_child(root, zz_string(&tree, 1, "314"));
	zz_append_child(root, zz_pointer(&tree, 2, &tree));

	assert(zz_match(root, 0) == 0);
	node = zz_first_child(root);
	assert(zz_match(node, 0) == 0);
	node = zz_next(node);
	assert(zz_match(node, 1) == 0);
	node = zz_next(node);
	assert(zz_match(node, 2) == 0);
	node = zz_next(node);
	assert(zz_match(node, 0) == 0);
	node = zz_next(node);
	assert(zz_match(node, 1) == 0);
	node = zz_next(node);
	assert(zz_match(node, 2) == 0);
	node = zz_next(node);
	assert(zz_match_end(node, &root->children) == 0);

	assert(zz_match(root, 1) != 0);
	node = zz_first_child(root);
	assert(zz_match(node, 1) != 0);
	node = zz_next(node);
	assert(zz_match(node, 2) != 0);
	node = zz_next(node);
	assert(zz_match(node, 0) != 0);
	node = zz_next(node);
	assert(zz_match(node, 1) != 0);
	node = zz_next(node);
	assert(zz_match(node, 2) != 0);
	node = zz_next(node);
	assert(zz_match(node, 0) != 0);

	node = zz_first_child(root);
	assert(zz_match_end(node, &root->children) != 0);
	node = zz_next(node);
	assert(zz_match_end(node, &root->children) != 0);
	node = zz_next(node);
	assert(zz_match_end(node, &root->children) != 0);
	node = zz_next(node);
	assert(zz_match_end(node, &root->children) != 0);
	node = zz_next(node);
	assert(zz_match_end(node, &root->children) != 0);
	node = zz_next(node);
	assert(zz_match_end(node, &root->children) != 0);

	exit(EXIT_SUCCESS);
}
