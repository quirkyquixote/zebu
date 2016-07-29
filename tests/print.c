
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
	struct zz_node *root;
	struct zz_node *node;

	zz_tree_init(&tree, node_types, sizeof(node_types) / sizeof(*node_types));

	assert((root = zz_inner(&tree, 6, zz_list_empty)) != NULL);

	zz_append(root, zz_null(&tree, 0));
	zz_append(root, zz_int(&tree, 1, -314));
	zz_append(root, zz_uint(&tree, 2, 314));
	zz_append(root, zz_double(&tree, 3, 3.14));
	zz_append(root, zz_string(&tree, 4, "314"));
	zz_append(root, zz_pointer(&tree, 5, &tree));
	zz_append(root, zz_inner(&tree, 6, zz_list_empty));

	zz_print_node(root, stdout);
	printf("\n");
	exit(EXIT_SUCCESS);
}
