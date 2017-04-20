
#ifndef ZEBU_PRINT_H_
#define ZEBU_PRINT_H_

#include "node.h"

/*
 * Print the full tree whose root is _node_ to _f_ 
 *
 * @node a zz_node
 * @f a FILE
 */
void zz_print(struct zz_node *node, FILE * f);

/*
 * Print error message
 *
 * Prints an error message including the file name _file_ and line _line_, then
 * prints the offending line and a caret pointing at the offending column.
 *
 * @msg error messabe to be printed
 * @file path to the file with the error
 * @line offending line
 * @first_column first offending column
 * @last_column last offending column
 */
void zz_error(const char *msg, const char *file, size_t first_line,
		size_t first_column, size_t last_line, size_t last_column);

#endif       // ZEBU_PRINT_H_
