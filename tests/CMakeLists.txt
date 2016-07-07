
add_executable(tree_test tree_test.c)
target_link_libraries(tree_test zebu)
add_test(NAME tree COMMAND tree_test)

add_executable(print_test print_test.c)
target_link_libraries(print_test zebu)
add_test(NAME print COMMAND print_test)

add_executable(walk_test walk_test.c)
target_link_libraries(walk_test zebu)
add_test(NAME walk COMMAND walk_test)
