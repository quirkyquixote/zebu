/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */

#ifndef ZEBU_PRINT_H_
#define ZEBU_PRINT_H_

#include "node.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Print
 * -----
 */

/**
 * Print the full tree whose root is ``node`` to ``f`` 
 */
void zz_print(struct zz_node *node, FILE *f);

/**
 * Print error message
 *
 * Prints an error message including the file name ``file`` and line ``line``,
 * then prints the offending line and a caret pointing at the offending column.
 */
void zz_error(const char *msg, const char *file, size_t first_line,
		size_t first_column, size_t last_line, size_t last_column);

#ifdef __cplusplus
}
#endif

#endif       // ZEBU_PRINT_H_
