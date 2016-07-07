
add_executable(ast_test ast_test.c ../ast.c)
add_test(NAME ast COMMAND ast_test)
