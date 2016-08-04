
#include <assert.h>
#include <string.h>

#include "../zebu.h"

static const char *TOK_FOO = "foo";
static const char *TOK_BAR = "bar";
static const char *TOK_BAZ = "baz";

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *node;

	zz_tree_init(&tree, sizeof(struct zz_node));

	assert((node = zz_null(&tree, TOK_FOO)) != NULL);

	assert((node = zz_int(&tree, TOK_BAR, -314)) != NULL);
	assert(zz_to_int(node) == -314);

	assert((node = zz_uint(&tree, TOK_BAZ, 314)) != NULL);
	assert(zz_to_uint(node) == 314);

	assert((node = zz_double(&tree, TOK_FOO, 3.14)) != NULL);
	assert(zz_to_double(node) == 3.14);

	assert((node = zz_string(&tree, TOK_BAR, "314")) != NULL);
	assert(strcmp(zz_to_string(node), "314") == 0);

	assert((node = zz_pointer(&tree, TOK_BAZ, &tree)) != NULL);
	assert(zz_to_pointer(node) == &tree);

	exit(EXIT_SUCCESS);
}
