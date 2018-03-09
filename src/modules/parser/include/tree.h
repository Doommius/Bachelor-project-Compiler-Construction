#ifndef __tree_h
#define __tree_h

#include "kind.h"
#include "symbol.h"

typedef struct function {
    symbol_table *table;
    int lineno;
    struct head *head;
    struct body *body;
    struct tail *tail;

} function;

typedef struct head {
    symbol_table *table;
    int lineno;
    char *id;
    struct par_decl_list *list;
    struct type *type;

} head;

typedef struct tail {
    symbol_table *table;
    int lineno;
    char *id;
} tail;

typedef struct type {
    symbol_table *table;
    int lineno;
    TYPE_kind kind;
    union {
        char *id;
        struct type *type;
        struct var_decl_list *list;
    } val;
} type;

typedef struct par_decl_list {
    symbol_table *table;
    int lineno;
    PDL_kind kind;
    struct var_decl_list *list;
} par_decl_list;

typedef struct var_decl_list {
    symbol_table *table;
    int lineno;
    VDL_kind kind;
    struct var_decl_list *list;
    struct var_type *vartype;
} var_decl_list;

typedef struct var_type {
    symbol_table *table;
    int lineno;
    char *id;
    struct type *type;
} var_type;

typedef struct body {
    symbol_table *table;
    int lineno;
    struct decl_list *d_list;
    struct statement_list *s_list;
} body;

typedef struct decl_list {
    symbol_table *table;
    int lineno;
    DL_kind kind;
    struct declaration *decl;
    struct decl_list *list;
} decl_list;

typedef struct declaration {
    symbol_table *table;
    int lineno;
    DECL_kind kind;
    union {
        struct {
            char *id;
            struct type *type;
        } type;
        struct function *function;
        var_decl_list *list;
    } val;

} declaration;

typedef struct statement_list {
    symbol_table *table;
    int lineno;
    SL_kind kind;
    struct statement *statement;
    struct statement_list *list;

} statement_list;

typedef struct statement {
    symbol_table *table;
    int lineno;
    STATEMENT_kind kind;
    union {
        struct expression *ret;
        struct expression *wrt;
        struct {
            struct variable *variable;
            struct expression *length;
        } allocate;

        struct {
            struct variable *variable;
            struct expression *expression;
        } assignment;

        struct {
            struct expression *expression;
            struct statement *statement1;
            struct statement *statement2;
        } ifthen;

        struct {
            struct expression *expression;
            struct statement *statement;
        } loop;

        struct statement_list *list;
    } val;

} statement;

typedef struct variable {
    symbol_table *table;
    int lineno;
    char *id;
    Var_kind kind;
    union {
        struct {
            struct variable *var;
            struct expression *exp;
        } exp;
        struct {
            struct variable *var;
            char *id;
        } record;
    } val;
} variable;

typedef struct expression {
    symbol_table *table;
    int lineno;
    EXP_kind kind;
    union {
        struct {
            struct expression *left;
            struct expression *right;
        } ops;
        struct term *term;
        struct expression *neg;
    } val;

} expression;

typedef struct term {
    symbol_table *table;
    int lineno;
    TERM_kind kind;
    union {
        int num;
        struct expression *expression;
        struct term *term_not;
        struct variable *variable;
        struct {
            char *id;
            struct act_list *list;
        } list;
    } val;
} term;

typedef struct act_list {
    symbol_table *table;
    int lineno;
    AL_kind kind;
    struct exp_list *list;
} act_list;

typedef struct exp_list {
    symbol_table*table;
    int lineno;
    EL_kind kind;
    struct expression *expression;
    struct exp_list *list;
} exp_list;

function *make_Func(head *h, body *b, tail *t);

head *make_Head(char *id, par_decl_list *pdl, type *t);

tail *make_Tail(char *id);

type *make_Type_id(char *id);
type *make_Type_int();
type *make_Type_bool();
type *make_Type_array(type *t);
type *make_Type_record(var_decl_list *vdl);

par_decl_list *make_PDL_list(var_decl_list *vdl);
par_decl_list *make_PDL_empty();

var_decl_list *make_VDL_list(var_type *vt, var_decl_list *vdl);
var_decl_list *make_VDL_type(var_type *vt);

var_type *make_VType_id(char *id, type *t);

body *make_Body(decl_list *dl, statement_list *sl);

decl_list *make_DL_list(declaration *d, decl_list *dl);
decl_list *make_DL_empty();

declaration *make_Decl_type(char *id, type *t);
declaration *make_Decl_func(function *f);
declaration *make_Decl_list(var_decl_list *vdl);

statement_list *make_SL_statement(statement *s);
statement_list *make_SL_list(statement *s, statement_list *sl);

statement *make_STMT_ret(expression *e);
statement *make_STMT_wrt(expression *e);
statement *make_STMT_allocate_var(variable *v);
statement *make_STMT_allocate_length(variable *v, expression *e);
statement *make_STMT_assign(variable *v, expression *e);
statement *make_STMT_if(expression *e, statement *s);
statement *make_STMT_if_else(expression *e, statement *s1, statement *s2);
statement *make_STMT_while(expression *e, statement *s);
statement *make_STMT_list(statement_list *sl);

variable *make_Var_id(char *id);
variable *make_Var_exp(variable *var, expression *expression);
variable *make_Var_record(variable *var, char *id);

expression *make_EXP(EXP_kind kind, expression *left, expression *right);
expression *make_EXP_term(term *term);
expression *make_EXP_neg(expression *neg);

term *make_Term_num(int intconst);
term *make_Term_par(expression *expression);
term *make_Term_not(term *term);
term *make_Term_abs(expression *expression);
term *make_Term_boolean(int bool);
term *make_Term_null();
term *make_Term_variable(variable *var);
term *make_Term_list(char *id, act_list *list);

act_list *make_Act_list(exp_list *list);
act_list *make_Act_empty();

exp_list *make_ExpL_exp(expression *expression);
exp_list *make_ExpL_list(expression *expression, exp_list *list);

int check_Func(head *head, tail *tail);

#endif