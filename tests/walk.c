
#include <assert.h>

#include "../zebu.h"

static const char *TOK_FOO = "foo";
static const char *TOK_BAR = "bar";
static const char *TOK_BAZ = "baz";

static void error(void *node, const char *msg)
{
	fprintf(stderr, "error: %s\n", msg);
}

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *root;
	struct zz_node *node;

	zz_tree_init(&tree, sizeof(struct zz_node));

	assert((root = zz_null(&tree, TOK_FOO)) != NULL);

	zz_append_child(root, zz_null(&tree, TOK_FOO));
	zz_append_child(root, zz_int(&tree, TOK_BAR, -314));
	zz_append_child(root, zz_uint(&tree, TOK_BAZ, 314));
	zz_append_child(root, zz_double(&tree, TOK_FOO, 3.14));
	zz_append_child(root, zz_string(&tree, TOK_BAR, "314"));
	zz_append_child(root, zz_pointer(&tree, TOK_BAZ, &tree));

	assert(zz_match(root, NULL, TOK_FOO, error) == 0);
	node = zz_first_child(root);
	assert(zz_match(node, root, TOK_FOO, error) == 0);
	node = zz_next(node);
	assert(zz_match(node, root, TOK_BAR, error) == 0);
	node = zz_next(node);
	assert(zz_match(node, root, TOK_BAZ, error) == 0);
	node = zz_next(node);
	assert(zz_match(node, root, TOK_FOO, error) == 0);
	node = zz_next(node);
	assert(zz_match(node, root, TOK_BAR, error) == 0);
	node = zz_next(node);
	assert(zz_match(node, root, TOK_BAZ, error) == 0);
	node = zz_next(node);
	assert(zz_match_end(node, root, error) == 0);

	assert(zz_match(root, NULL, TOK_BAR, error) != 0);
	node = zz_first_child(root);
	assert(zz_match(node, root, TOK_BAR, error) != 0);
	node = zz_next(node);
	assert(zz_match(node, root, TOK_BAZ, error) != 0);
	node = zz_next(node);
	assert(zz_match(node, root, TOK_FOO, error) != 0);
	node = zz_next(node);
	assert(zz_match(node, root, TOK_BAR, error) != 0);
	node = zz_next(node);
	assert(zz_match(node, root, TOK_BAZ, error) != 0);
	node = zz_next(node);
	assert(zz_match(node, root, TOK_FOO, error) != 0);

	node = zz_first_child(root);
	assert(zz_match_end(node, root, error) != 0);
	node = zz_next(node);
	assert(zz_match_end(node, root, error) != 0);
	node = zz_next(node);
	assert(zz_match_end(node, root, error) != 0);
	node = zz_next(node);
	assert(zz_match_end(node, root, error) != 0);
	node = zz_next(node);
	assert(zz_match_end(node, root, error) != 0);
	node = zz_next(node);
	assert(zz_match_end(node, root, error) != 0);

	exit(EXIT_SUCCESS);
}
