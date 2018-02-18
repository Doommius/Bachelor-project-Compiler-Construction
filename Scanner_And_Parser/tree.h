#ifndef __tree_h
#define __tree_h

#include "kind.h"
//#define EMPTY 0
//#define LIST 1
//#define TYPE 2



typedef struct function{
    int lineno;
    struct head *head;
    struct body *body;
    struct tail *tail;
    //TODO Der skal være noget mere her

} function;

typedef struct head{
    int lineno;
    char *id;
    struct par_decl_list *list;
    struct type *type;

} head;

typedef struct tail{
    int lineno;
    char *id;
} tail;

typedef struct type {
    int lineno;
    TYPE_kind kind;
    union {
        char *id;
        //TODO der skal være noget mere her
    } val;
} type;

//TO
typedef struct par_decl_list{
    int lineno;
    PDL_kind kind;
    struct var_decl_list *list;
} par_decl_list;

typedef struct var_decl_list{
    int lineno;
    VDL_kind kind;
    struct var_decl_list *list;
    struct var_type *vartype;
} var_decl_list;

typedef struct var_type{
    int lineno;
    char *id;
    struct type *type;
} var_type;

typedef struct body{
    int lineno;
    struct decl_list *d_list;
    struct statement_list *s_list;
} body;

typedef struct decl_list{
    int lineno;
    DL_kind kind;
    struct declaration *decl;
    struct decl_list *list;
} decl_list;

typedef struct declaration{
    int lineno;
    DECL_kind kind;
    //TODO ved ikke lige med det der type id = <type>, det her er midlertidigt
    struct type *type;
    struct function *function;
    struct var_decl_list *list;

} declaration;

typedef struct statement_list{
    int lineno;
    struct statement *statement;
    struct statement_list *list;

} statement_list;

typedef struct statement{
    int lineno;
    //TODO Sikkert ikke rigtigt
    STATEMENT_kind kind;

} statement;

typedef struct variable{
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
        } varid;
    } val;
    //TODO Skal være noget mere her
} variable;

typedef struct expression{
    int lineno;
    EXP_kind kind;
    union {
        struct {
            struct expression *left;
            struct expression *right;
        } ops;
        struct term *term;
    } val;

    //TODO Skal være noget mere her
} expression;

typedef struct term{
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
        //TODO Skal være noget mere her
    } val;
} term;

typedef struct act_list{
    int lineno;
    AL_kind kind;
    struct exp_list *list;
} act_list;

typedef struct exp_list{
    int lineno;
    EL_kind kind;
    struct expression *expression;
    struct exp_list *list;
} exp_list;



type *make_type_id(char *id);

variable *make_Var_id(char *id);
variable *make_Var_exp(variable *var, expression *expression);
variable *make_Var_varid(variable *var, char *id);

expression *make_EXP(EXP_kind kind, expression *left, expression *right);
expression *make_EXP_term(term *term);

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


//EXP *makeEXPArithmeticstructure(EXP *left, EXP *right, kindArithmetic kind);


#endif
