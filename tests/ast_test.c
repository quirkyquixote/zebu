
#include "../ast.h"

static const struct ast_node_type node_types[] = {
    { "null", AST_NULL },
    { "int", AST_INT },
    { "uint", AST_UINT },
    { "double", AST_DOUBLE },
    { "string", AST_STRING },
    { "inner", AST_INNER },
};

int main(int argc, char *argv[])
{
    struct ast_mgr mgr;
    struct ast_node *node;

    ast_mgr_init(&mgr, node_types, sizeof(node_types) / sizeof(*node_types));

    assert((node = ast_null(mgr)) != NULL);

    assert((node = ast_int(mgr, -314)) != NULL);
    assert(ast_to_int(node) == -314);

    assert((node = ast_uint(mgr, 314)) != NULL);
    assert(ast_to_uint(node) == 314);

    assert((node = ast_double(mgr, 3.14)) != NULL);
    assert(ast_to_double(node) == 3.14);

    assert((node = ast_string(mgr, "314")) != NULL);
    assert(strcmp(ast_to_string(node), "314") == 0)

    assert((node = ast_inner(mgr, ast_list_empty)) != NULL)
    assert(ast_to_list(node) == ast_list_empty);

    exit(EXIT_SUCCESS);
}
