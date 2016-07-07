/* Generic parser.
 *
 * Oct 2013: Created by Luis Sanz
 */

#include "parser.h"

int parse_file(struct ast_parser *p, struct ast_manager *m, const char *filename, struct ast_node **ast, GError **err)
{
    struct ast_parserState s = {
        .filename = filename,
        .errors = 0,
        .ast = NULL,
        .ast_mgr = m,
        .err = NULL
    };

    int retval = -1;
    FILE *file;
    void *scanner;

    if ((file = fopen(filename, "r")) != NULL) {
        if (p->lex_init(&scanner) == 0) {
            p->set_in(file, scanner);
            if ((retval = p->parse(&s, scanner)) == 0) {
                if (ast)
                    *ast = s.ast;
            } else {
                g_propagate_error(err, s.err);
            }
            p->lex_destroy(scanner);
        } else {
            g_set_error_literal(err, FTU_ERROR, 0, "Can't initialize scanner");
        }

        fclose(file);
    } else {
        propagate_system_error(err, FTU_ERROR, 0, filename);
    }

    return retval;
}

void ast_print(struct ast_node * ast, FILE * f)
{
    const struct ast_node_type *tt = &ast->mgr->token_types[ast->token];
    fprintf(f, "[%s", tt->name);

    switch (tt->type) {
        case FTU_NULL_AST:
            break;
        case FTU_INT_AST:
            fprintf(f, " %d", ast->data.sint);
            break;
        case FTU_UINT_AST:
            fprintf(f, " %u", ast->data.uint);
            break;
        case FTU_FLOAT_AST:
            fprintf(f, " %f", ast->data.real);
            break;
        case FTU_STRING_AST:
            fprintf(f, " \"%s\"", ast->data.string);
            break;
        case FTU_INNER_AST:
            ast_list_print(ast->data.list, f);
            break;
    }

    fprintf(f, "]");
}

void ast_list_print(struct ast_node_list list, FILE * f)
{
    struct ast_node *it;

    if (it = list.first) {
        for (;;) {
            fprintf(f, " ");
            ast_print(it, f);
            if ((it = it->next) == NULL)
                break;
        }
    }
}

int ast_match_at_line(struct ast_node * ast, int tok, const char *file, int line)
{
    if (ast == NULL) {
        fprintf(stderr, "%s:%d: unexpected end of node list", file, line);
        return -1;
    } else if (ast->token != tok) {
        fprintf(stderr, "%s:%d: expected %s, got %s", file, line,
                ast_manager_token_type(ast->mgr, tok)->name,
                ast_token_type(ast)->name);
        return -1;
    }
    return 0;
}

int ast_match_end_at_line(struct ast_node * ast, const char *file, int line)
{
    if (ast != NULL) {
        fprintf(stderr, "%s:%d: unexpected AST node: %s",
                file, line, ast_token_type(ast)->name);
        return -1;
    }
    return 0;
}
