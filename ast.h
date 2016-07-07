/* @file
 *  Abstract Syntax Tree
 *
 *  Handles the creation and parsing of Abstract Syntax Trees. The usual
 *  schema of a program using this is as follows:
 *
 *  @code{.c}
 *  const struct ast_node_types tt[] = {
 *      { "token-name-1", AST_INNER },
 *      { "token-name-2", AST_INNER },
 *      { "token-name-3", AST_STRING },
 *      { "token-name-4", AST_STRING },
 *  };
 *
 *  struct ast_mgr *mgr;
 *  struct ast_node *ast;
 *
 *  if (mgr = ast_mgr_new(tt)) {
 *      if (ast = create_ast_from_file(mgr, file)) {
 *          // walk ast here
 *      }
 *      ast_mgr_free(mgr);
 *  }
 *  @endcode
 *
 *  The create_ast_from_file() function uses all the *_ast_new methods to
 *  build the return value from tokens read from file.
 *
 *  @author Jan 2013: Created by Luis Sanz
 */

#ifndef AST_BISON_UTILS_H_
#define AST_BISON_UTILS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

enum {
    AST_NULL,        /*< Node with no information */
    AST_INT,         /*< Node with an integer */
    AST_UINT,        /*< Node with an unsigned integer */
    AST_FLOAT,       /*< Node with a floating point number */
    AST_STRING,      /*< Node with a string */
    AST_INNER,       /*< Node with more nodes */
};

/* Opaque structure used to allocate AST nodes and related strings.
 * Keeps a record of all allocated elements and frees then when destroyed.*/
struct ast_mgr;

/* AST node type */
struct ast_node_type {
    const char *name;   /*< Name for logging purposes */
    int type;           /*< Integer identifier used in AST nodes */
};

/* AST list */
struct ast_list {
    struct ast_node *first;       /*< First node in the list */
    struct ast_node *last;        /*< Last node in the list */
};

/* AST node */
struct ast_node {
    struct ast_mgr *mgr; /*< Manager to which his node belongs */
    int token;          /*< Token for this node */
    struct ast_node *next;        /*< Next sibling */
    union {
        unsigned int uint;
        int sint;
        double real;
        char *string;
        struct ast_list list;
    } data;
};

/* Create new ast_mgr */
void ast_mgr_init(struct ast_mgr *mgr, const struct ast_node_type *token_types,
        size_t token_types_size);
/* Destroy existing ast_mgr */
void ast_mgr_deinit(struct ast_mgr *mgr);
/* Get token type for this manager or "<bad>" if unexistant */
const struct ast_node_type *ast_mgr_token_type(struct ast_mgr *mgr, int tok);
/* Get token type for this token */
const struct ast_node_type *ast_token_type(struct ast_node *ast);
/* Get token name for this manager or "<bad>" if unexistant */
const char *ast_mgr_token_name(struct ast_mgr *mgr, int tok);
/* Get token name for this token */
const char *ast_token_name(struct ast_node *ast);

/* Copy a struct ast_node node belonging to mgr */
struct ast_node *ast_copy(struct ast_mgr *mgr, struct ast_node *node);
/* Copy a struct ast_node node and all its children belonging to mgr */
struct ast_node *ast_copy_recursive(struct ast_mgr *mgr, struct ast_node *node);

/* Create a new empty node */
struct ast_node *ast_null(struct ast_mgr *mgr, int tok);
/* Create a new node with integer data */
struct ast_node *ast_int(struct ast_mgr *mgr, int tok, int val);
/* Create a new node with unsigned integer data */
struct ast_node *ast_uint(struct ast_mgr *mgr, int tok, unsigned int val);
/* Create a new node with float data */
struct ast_node *ast_double(struct ast_mgr *mgr, int tok, double val);
/* Create a new node with string data */
struct ast_node *ast_string(struct ast_mgr *mgr, int tok, const char *val);
/* Create a new node from which mode nodes may hang */
struct ast_node *ast_inner(struct ast_mgr *mgr, int tok, struct ast_list children);

/* Return integer data associated with ast or raise an error */
int ast_to_int(struct ast_node *ast);
/* Return unsigned integer data associated with ast or raise an error */
unsigned int ast_to_uint(struct ast_node *ast);
/* Return float data associated with ast or raise an error */
double ast_to_double(struct ast_node *ast);
/* Return string data associated with ast or raise an error */
const char *ast_to_string(struct ast_node *ast);
/* Return list data associated with ast */
struct ast_list ast_to_list(struct ast_node *ast);

/* Remove all children from ast or raise an error */
int ast_clear(struct ast_node *ast);
/* Append child to t children list or raise an error */
int ast_append(struct ast_node *t, struct ast_node *child);
/* Prepend child to t children list or raise an error */
int ast_prepend(struct ast_node *t, struct ast_node *child);
/* Insert child after prev in t children list or raise an error */
int ast_insert(struct ast_node *t, struct ast_node *prev, struct ast_node *child);
/* Append all nodes in list to t children or raise an error */
int ast_append_list(struct ast_node *t, struct ast_list children);
/* Prepend all nodes in list to t children or raise an error */
int ast_prepend_list(struct ast_node *t, struct ast_list children);

/* Empty struct ast_list instances may be created by assigning this */
extern const struct ast_list ast_list_empty;
/* Create a new struct ast_list from the @c NULL terminated argument list */
struct ast_list ast_list(struct ast_node *t, ...);
/* Return the struct ast_list that results from appendind t to list;
 * list is invalidated */
struct ast_list ast_list_append(struct ast_list list, struct ast_node *t);
/* Return the struct ast_list that results from prependind t to list;
 * list is invalidated */
struct ast_list ast_list_prepend(struct ast_list list, struct ast_node *t);
/* Return the struct ast_list that results from inserting t after prev in
 * list; list is invalidated */
struct ast_list ast_list_insert(struct ast_list list, struct ast_node *prev,
        struct ast_node *t);
/* Returm the struct ast_list that results from concatenating left and right;
 * both left and right are invalidated */
struct ast_list ast_list_concat(struct ast_list left, struct ast_list right);
/* Return the element of list whose index is i */
struct ast_node *ast_list_index(struct ast_list list, int i);

#endif
