/** @file
 *  Wrapper for Flex+Bison parsers.
 *
 *  The usual usage for this module is as follows. Assuming a parser exists
 *  with the following methods:
 *
 *  @code
 *  int yylex_init(yyscan_t *);
 *  void yylex_destroy(yyscan_t);
 *  void yyset_in(FILE *, yyscan_t);
 *  int yyparse(struct ast_parserState *p, yyscan_t scanner);
 *  @endcode
 *
 *  You can create and use it wrapping it like this:
 *
 *  @code
 *  struct ast_parser my_parser = {
 *      .lex_init = yylex_init,
 *      .lex_destroy = yylex_destroy,
 *      .set_in = yyset_in,
 *      .parse = yyparse,
 *  };
 *
 *  FtuTokenType tt[] = { ... };
 *  struct ast_manager *mgr;
 *  struct ast_node *ast;
 *
 *  if (mgr = ast_manager_new(tt)) {
 *      if (ast = parse_file(my_parser, mgr, path)) {
 *          // success!
 *      }
 *      ast_manager_free(mgr);
 *  }
 *  @endcode
 *
 *  Note that the `yyparse()` method is responsible for creating the AST and
 *  returning it correctly. This is done by setting the ast field of the
 *  struct ast_parserState variable passed to it to the root node of the tree that must
 *  be returned.
 *
 *  It is a <b>bad idea</b> to use the `ast_manager_raise()` method inside
 *  `yyparse` to reach a `ast_manager_try()` outside it: this would prevent the
 *  parser utilities from being correctly deinitialized.
 *
 *  @author Oct 2013: Created by Luis Sanz
 */

#ifndef FTU_BISON_UTILS_PARSER_H_
#define FTU_BISON_UTILS_PARSER_H_

#include "ast.h"

/** Internal state of a parser */
typedef struct _struct ast_parserState {
    const char *filename;       /**< Path to the file being parsed*/
    int errors;                 /**< Number of errors*/
    struct ast_node *ast;                 /**< Root of the AST to be generated*/
    struct ast_manager *ast_mgr;      /**< Produces all AST nodes and strings*/
    GError *err;                /**< Contains an error, if any */
} struct ast_parserState;

/** A generic parser */
typedef struct _struct ast_parser {
    int (*lex_init) (void **);  /**< Initialize lexer*/
    void (*lex_destroy) (void *);       /**< Destroy lexed*/
    void (*set_in) (FILE *, void *);    /**< Set input stream*/
    int (*parse) (struct ast_parserState *, void *);   /**< Actually parse*/
} struct ast_parser;

/** Parse a file
    @param p A valid struct ast_parser, contains methods implementing parser and lexer
    @param m A valid struct ast_manager, capable of creating AST nodes and strings
    @param filename Path to the file to parse
    @return The root node of the AST on sucess; NULL on error */
int parse_file(struct ast_parser *p, struct ast_manager *m, const char *filename, struct ast_node **ast, GError **err);

/* Print the full tree whose root is `ast` to `f` */
void ast_print(struct ast_node * ast, FILE * f);
/* Print the full forest whose roots are `list` to `f` */
void ast_list_print(struct ast_node_list list, FILE * f);

/* If the token for `ast` is not `tok`, raise an error whose message begins with `file`:`line`: */
int ast_match_at_line(struct ast_node * ast, int tok, const char * file, int line);
/* If `ast` is not @c NULL, raise an error whose message begins with `file`:`line`: */
int ast_match_end_at_line(struct ast_node * ast, const char *file, int line);
/* A macro that calls ast_match_at_line with `__FILE__` and `__LINE__` as arguments */
#define ast_match(ast,tok,err) ast_match_at_line(ast,tok,__FILE__,__LINE__,err)
/* A macro that calls ast_match_end_at_line with `__FILE__` and `__LINE__` as arguments */
#define ast_match_end(ast,err) ast_match_end_at_line(ast,__FILE__,__LINE__,err)
/* If the token for `ast` is not `tok`, raise an error with `message` */
int ast_match_with_message(struct ast_node * ast, int tok, const char * message);
/* If `ast` is not @c NULL, raise an error with `message` */
int ast_match_end_with_message(struct ast_node * ast, const char * message);

#endif  // FTU_BISON_UTILS_PARSER_H_
