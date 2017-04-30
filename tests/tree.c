
#include <assert.h>
#include <string.h>

#include "../src/zebu.h"

static const char *TOK_FOO = "foo";
static const char *TOK_BAR = "bar";
static const char *TOK_BAZ = "baz";

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *node;

	zz_tree_init(&tree, sizeof(struct zz_node));

	assert((node = zz_node(&tree, TOK_FOO, zz_null)) != NULL);

	assert((node = zz_node(&tree, TOK_BAR, zz_int(-314))) != NULL);
	assert(zz_get_int(node) == -314);

	assert((node = zz_node(&tree, TOK_BAZ, zz_uint(314))) != NULL);
	assert(zz_get_uint(node) == 314);

	assert((node = zz_node(&tree, TOK_FOO, zz_double(0.5))) != NULL);
	assert(zz_get_double(node) == 0.5);

	assert((node = zz_node(&tree, TOK_BAR, zz_string("314"))) != NULL);
	assert(strcmp(zz_get_string(node), "314") == 0);

	assert((node = zz_node(&tree, TOK_BAZ, zz_pointer(&tree))) != NULL);
	assert(zz_get_pointer(node) == &tree);

	exit(EXIT_SUCCESS);
}
