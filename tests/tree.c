
#include <assert.h>

#include "../zebu.h"

static const struct zz_node_type node_types[] = {
	{ "null", ZZ_NULL },
	{ "int", ZZ_INT },
	{ "uint", ZZ_UINT },
	{ "double", ZZ_DOUBLE },
	{ "string", ZZ_STRING },
	{ "pointer", ZZ_POINTER },
	{ "inner", ZZ_INNER },
};

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *node;

	zz_tree_init(&tree, node_types, sizeof(node_types) / sizeof(*node_types));

	assert((node = zz_null(&tree, 0)) != NULL);

	assert((node = zz_int(&tree, 1, -314)) != NULL);
	assert(zz_to_int(node) == -314);

	assert((node = zz_uint(&tree, 2, 314)) != NULL);
	assert(zz_to_uint(node) == 314);

	assert((node = zz_double(&tree, 3, 3.14)) != NULL);
	assert(zz_to_double(node) == 3.14);

	assert((node = zz_string(&tree, 4, "314")) != NULL);
	assert(strcmp(zz_to_string(node), "314") == 0);

	assert((node = zz_pointer(&tree, 5, &tree)) != NULL);
	assert(strcmp(zz_to_pointer(node), &tree) == 0);

	assert((node = zz_inner(&tree, 6, zz_list_empty)) != NULL);
	assert(zz_children(node) == zz_list_empty.first);

	exit(EXIT_SUCCESS);
}
