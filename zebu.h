
#ifndef ZEBU_H_
#define ZEBU_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define ZZ_CHUNK_SIZE 64

enum {
    ZZ_NULL,        /* Node with no information */
    ZZ_INT,         /* Node with an integer */
    ZZ_UINT,        /* Node with an unsigned integer */
    ZZ_DOUBLE,       /* Node with a floating point number */
    ZZ_STRING,      /* Node with a string */
    ZZ_POINTER,       /* Node with user-managed data */
    ZZ_INNER        /* Node with more nodes */
};

/* AST node type */
struct zz_node_type {
    const char *name;   /* Name for logging purposes */
    int type;           /* Integer identifier used in AST nodes */
};

/* AST list */
struct zz_list {
    struct zz_node *first;       /* First node in the list */
    struct zz_node *last;        /* Last node in the list */
};

/* AST node */
struct zz_node {
    struct zz_tree *tree; /* Manager to which his node belongs */
    int token;          /* Token for this node */
    struct zz_node *next;        /* Next sibling */
    union {
        unsigned int uint_val;
        int int_val;
        double double_val;
        const char *str_val;
        void *ptr_val;
        struct zz_list list_val;
    } data;
};

/* Allocate nodes */
struct zz_node_chunk {
    struct zz_node_chunk *next;
    struct zz_node data[ZZ_CHUNK_SIZE];
};

/* Allocate strings */
struct zz_string_chunk {
    struct zz_string_chunk *next;
    char *data[ZZ_CHUNK_SIZE];
};

/* Manage a full tree */
struct zz_tree {
    const struct zz_node_type *token_types;
    size_t token_types_size;
    struct zz_node_chunk *nodes;
    size_t node_chunk_size;
    struct zz_string_chunk *strings;
    size_t string_chunk_size;
};

/* Initialize tree */
void zz_tree_init(struct zz_tree *tree, const struct zz_node_type *token_types,
        size_t token_types_size);
/* Deinitialize tree */
void zz_tree_deinit(struct zz_tree *tree);

/* Get node type for token in tree, "<bad>" if nonexistant */
int zz_tree_token_type(struct zz_tree *tree, int tok);
/* Get node type for node */
int zz_token_type(struct zz_node *node);
/* Get token name for token, or "<bad>" if unexistant */
const char *zz_tree_token_name(struct zz_tree *tree, int tok);
/* Get token name for node */
const char *zz_token_name(struct zz_node *node);

/* Copy a node */
struct zz_node *zz_copy(struct zz_tree *tree, struct zz_node *node);
/* Copy a node and all its children recursively */
struct zz_node *zz_copy_recursive(struct zz_tree *tree, struct zz_node *node);

/* Create an empty node */
struct zz_node *zz_null(struct zz_tree *tree, int tok);
/* Create a node with int data */
struct zz_node *zz_int(struct zz_tree *tree, int tok, int val);
/* Create a node with unsigned int data */
struct zz_node *zz_uint(struct zz_tree *tree, int tok, unsigned int val);
/* Create a node with double data */
struct zz_node *zz_double(struct zz_tree *tree, int tok, double val);
/* Create a node with string data */
struct zz_node *zz_string(struct zz_tree *tree, int tok, const char *val);
/* Create a node with user-managed data data */
struct zz_node *zz_pointer(struct zz_tree *tree, int tok, void *val);
/* Create a nonterminal node  */
struct zz_node *zz_inner(struct zz_tree *tree, int tok, struct zz_list list);

/* Return integer data associated with node */
int zz_to_int(const struct zz_node *node);
/* Return unsigned integer data associated with node */
unsigned int zz_to_uint(const struct zz_node *node);
/* Return float data associated with node */
double zz_to_double(const struct zz_node *node);
/* Return string data associated with node */
const char *zz_to_string(const struct zz_node *node);
/* Return pointer data associated with node */
void *zz_to_pointer(const struct zz_node *node);

/* Return next sibling */
struct zz_node *zz_next(const struct zz_node *node);
/* Return first child */
struct zz_node *zz_children(const struct zz_node *node);
/* Remove all children from node */
void zz_clear(struct zz_node *node);
/* Append child to node */
void zz_append(struct zz_node *t, struct zz_node *child);
/* Prepend child to node */
void zz_prepend(struct zz_node *t, struct zz_node *child);
/* Insert child after prev in node */
void zz_insert(struct zz_node *t, struct zz_node *prev, struct zz_node *child);
/* Replace oldc by newc */
void zz_replace(struct zz_node *p, struct zz_node *oldc, struct zz_node *newc);
/* Append all nodes in list to node */
void zz_append_list(struct zz_node *t, struct zz_list list);
/* Prepend all nodes in list to node */
void zz_prepend_list(struct zz_node *t, struct zz_list list);

/* Copy to initialize empty lists */
extern const struct zz_list zz_list_empty;
/* Create a list from the NULL terminated argument list */
struct zz_list zz_list(struct zz_node *t, ...);
/* Return the list that results from appending node to list;
 * the original list is invalidated */
struct zz_list zz_list_append(struct zz_list list, struct zz_node *node);
/* Return the list that results from prepending node to list;
 * the original list is invalidated */
struct zz_list zz_list_prepend(struct zz_list list, struct zz_node *node);
/* Return the list that results from inserting node after prev in list;
 * the original list is invalidated */
struct zz_list zz_list_insert(struct zz_list list, struct zz_node *prev,
        struct zz_node *t);
/* Return the list that results from replacing oldc by newc in list;
 * the original list is invalidated */
struct zz_list zz_list_replace(struct zz_list list, struct zz_node *oldc,
        struct zz_node *newc);
/* Return the list that results from concatenating left and right;
 * both left and right are invalidated */
struct zz_list zz_list_concat(struct zz_list left, struct zz_list right);
/* Return the element of list whose index is i */
struct zz_node *zz_list_index(struct zz_list list, int i);

/* Print the full tree whose root is `node` to `f` */
void zz_print_node(struct zz_node * node, FILE * f);
/* Print the full forest whose roots are `list` to `f` */
void zz_print_list(struct zz_list list, FILE * f);

/* If the token for `node` is not `tok`, raise an error whose message begins with `file`:`line`: */
int zz_match_at_line(struct zz_node * node, int tok, const char * file, int line);
/* If `node` is not @c NULL, raise an error whose message begins with `file`:`line`: */
int zz_match_end_at_line(struct zz_node * node, const char *file, int line);
/* A macro that calls zz_match_at_line with `__FILE__` and `__LINE__` as arguments */
#define zz_match(node,tok) zz_match_at_line(node,tok,__FILE__,__LINE__)
/* A macro that calls zz_match_end_at_line with `__FILE__` and `__LINE__` as arguments */
#define zz_match_end(node) zz_match_end_at_line(node,__FILE__,__LINE__)

#endif
