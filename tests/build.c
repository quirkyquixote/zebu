

#include "../zebu.h"

enum {
	FOO_FUNC,
	FOO_TYPE,
	FOO_IDENT,
	FOO_ARGLIST,
	FOO_ARG,
	FOO_POINTER,
	FOO_BODY,
	FOO_CALL,
	FOO_STRING,
	FOO_NUM_TOKENS,
};

static const char *names[] = {
	"func",
	"type",
	"ident",
	"arglist",
	"arg",
	"pointer",
	"body",
	"call",
	"string",
};

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *n0, *n1, *n2, *n3, *n4, *n5;

	zz_tree_init(&tree, sizeof(struct zz_node), names, FOO_NUM_TOKENS);

	n0 = zz_null(&tree, FOO_FUNC);
	n1 = zz_string(&tree, FOO_TYPE, "int");
	zz_append_child(n0, n1);
	n1 = zz_string(&tree, FOO_IDENT, "main");
	zz_append_child(n0, n1);
	n1 = zz_null(&tree, FOO_ARGLIST);
	zz_append_child(n0, n1);
	n2 = zz_null(&tree, FOO_ARG);
	zz_append_child(n1, n2);
	n3 = zz_string(&tree, FOO_TYPE, "int");
	zz_append_child(n2, n3);
	n3 = zz_string(&tree, FOO_IDENT, "argc");
	zz_append_child(n2, n3);
	n2 = zz_null(&tree, FOO_ARG);
	zz_append_child(n1, n2);
	n3 = zz_null(&tree, FOO_POINTER);
	zz_append_child(n2, n3);
	n4 = zz_null(&tree, FOO_POINTER);
	zz_append_child(n3, n4);
	n5 = zz_string(&tree, FOO_TYPE, "int");
	zz_append_child(n4, n5);
	n3 = zz_string(&tree, FOO_IDENT, "argc");
	zz_append_child(n2, n3);
	n1 = zz_null(&tree, FOO_BODY);
	zz_append_child(n0, n1);
	n2 = zz_null(&tree, FOO_CALL);
	zz_append_child(n1, n2);
	n3 = zz_string(&tree, FOO_IDENT, "printf");
	zz_append_child(n2, n3);
	n3 = zz_null(&tree, FOO_ARGLIST);
	zz_append_child(n2, n3);
	n4 = zz_null(&tree, FOO_ARG);
	zz_append_child(n3, n4);
	n5 = zz_string(&tree, FOO_STRING, "Hello, World!");
	zz_append_child(n4, n5);

	zz_print_node(n0, stdout);
	fputc('\n', stdout);
	exit(EXIT_SUCCESS);
}

