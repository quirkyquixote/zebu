Usage
=====

A basic example in which an AST is built from a grammar similar to the one used
in the reverse polish notation calculator example in the GNU Bison
documentation

Declarations
------------

::

    %{
    /*
     * These are the tokens that define node types in the AST
     */
    extern const char *TOK_INPUT;
    extern const char *TOK_NUM;
    extern const char *TOK_ADD;
    extern const char *TOK_SUB;
    extern const char *TOK_MUL;
    extern const char *TOK_DIV;
    extern const char *TOK_EXP;
    extern const char *TOK_NEG;
    %}

    /*
     * The tree is a factory for nodes, and must be passed from outside.
     * Depending on your mode of operation, a return location for the root
     * node may be required, too.
     */
    %parse-param { struct zz_tree *tree }

    /*
     * At least one possible data type will be struct zz_node; all of them may
     * be, if the lexer has access to the zz_tree object and can construct
     * nodes by itself.
     */
    %union {
        int number;
        struct zz_node *node;
    }

    /*
     * Token and rule declarations
     */
    %token '\n'
    %token '+'
    %token '-'
    %token '*'
    %token '/'
    %token 'n'
    %token <number> NUM
    %type <ast> input
    %type <ast> line
    %type <ast> exp


Grammar Rules
-------------

::

    /*
     * To build a list of any size, the empty rule generates the root, and
     * every subsequent element found is added to the eny of its child list
     */
    input
        : {
            $$ = zz_node(tree, TOK_INPUT, zz_null);
            }
        | input line {
            $$ = $1;
            zz_append_child($$, $2);
            }
        ;

    /*
     * Pass the tree upwards
     */
    line
        : exp '\n' {
            return $$;
            }
        ;

    exp
        : NUM {
            $$ = zz_node(tree, TOK_NUM, zz_int($1));
            }
        | exp exp '+'     { 
            $$ = zz_node(tree, TOK_ADD, zz_null);
            zz_append_child($$, $1);
            zz_append_child($$, $2);
            }
        | exp exp '-'     { 
            $$ = zz_node(tree, TOK_SUB, zz_null);
            zz_append_child($$, $1);
            zz_append_child($$, $2);
            }
        | exp exp '*'     { 
            $$ = zz_node(tree, TOK_MUL, zz_null);
            zz_append_child($$, $1);
            zz_append_child($$, $2);
            }
        | exp exp '/'     { 
            $$ = zz_node(tree, TOK_DIV, zz_null);
            zz_append_child($$, $1);
            zz_append_child($$, $2);
            }
        | exp exp '^'     { 
            $$ = zz_node(tree, TOK_EXP, zz_null);
            zz_append_child($$, $1);
            zz_append_child($$, $2);
            }
        | exp 'n'         {
            $$ = zz_node(tree, TOK_NEG, zz_null);
            zz_append_child($$, $1);
            }
        ;


