
#include <stdio.h>

#include "../src/zebu.h"

static const char *FOO_FUNC = "func";
static const char *FOO_TYPE = "type";
static const char *FOO_IDENT = "ident";
static const char *FOO_ARGLIST = "arglist";
static const char *FOO_ARG = "arg";
static const char *FOO_POINTER = "pointer";
static const char *FOO_BODY = "body";
static const char *FOO_CALL = "call";
static const char *FOO_STRING = "string";

int main(int argc, char *argv[])
{
	struct zz_tree tree;
	struct zz_node *n0, *n1, *n2, *n3, *n4, *n5;

	zz_tree_init(&tree, sizeof(struct zz_node));

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

	setvbuf(stdout, NULL, _IONBF, 0);
	zz_print(n0, stdout);
	fputc('\n', stdout);
	exit(EXIT_SUCCESS);
}

