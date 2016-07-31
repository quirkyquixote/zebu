

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
	struct zz_node *n1, *n2, *n3, *n4;

	zz_tree_init(&tree, node_types, sizeof(node_types) / sizeof(*node_types));

	n1 = zz_string(&tree, 4, "foo");
	n2 = zz_string(&tree, 4, "bar");
	n3 = zz_string(&tree, 4, "foobar");
	n4 = zz_string(&tree, 4, "foo");

	assert(strcmp(zz_to_string(n1), "foo") == 0);
	assert(strcmp(zz_to_string(n2), "bar") == 0);
	assert(strcmp(zz_to_string(n3), "foobar") == 0);
	assert(strcmp(zz_to_string(n4), "foo") == 0);

	assert(zz_to_string(n1) == zz_to_string(n4));

	exit(EXIT_SUCCESS);
}
