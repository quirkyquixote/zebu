
#include <assert.h>
#include <string.h>

#include "../zebu.h"

static const char *names[] = { "foo", "bar", "baz" };

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *node;

	zz_tree_init(&tree, sizeof(struct zz_node), names, 3);

	assert((node = zz_null(&tree, 0)) != NULL);

	assert((node = zz_int(&tree, 1, -314)) != NULL);
	assert(zz_to_int(node) == -314);

	assert((node = zz_uint(&tree, 2, 314)) != NULL);
	assert(zz_to_uint(node) == 314);

	assert((node = zz_double(&tree, 0, 3.14)) != NULL);
	assert(zz_to_double(node) == 3.14);

	assert((node = zz_string(&tree, 1, "314")) != NULL);
	assert(strcmp(zz_to_string(node), "314") == 0);

	assert((node = zz_pointer(&tree, 2, &tree)) != NULL);
	assert(zz_to_pointer(node) == &tree);

	exit(EXIT_SUCCESS);
}
