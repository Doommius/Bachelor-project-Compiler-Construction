#ifndef __tree_h
#define __tree_h


typedef enum {idK,intconstK,timesK,divK,plusK,minusK, modK} kind;

typedef struct EXP {
    int lineno;
    kind kind;
    union {
        char *idE;
        int intconstE;
        struct {struct EXP *left; struct EXP *right;} val;
    } data;
} EXP;

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
    enum {type_ID, type_INT, type_BOOl, type_ARRAY, type_RECORD} kind;
    union {
        char *id;
        //TODO der skal være noget mere her
    } val;
} type;

typedef struct par_decl_list{
    int lineno;
    enum {LIST, EMPTY} kind;
    struct var_decl_list *list;
} par_decl_list;

typedef struct var_decl_list{
    int lineno;
    enum {LIST, TYPE} kind;
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
    enum {LIST, EMPTY} kind;
    struct declaration *decl;
    struct decl_list *list;
} decl_list;

typedef struct declaration{
    int lineno;
    enum {ID, FUNC, VAR} kind;
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
    enum {RETURN, WRITE, ALLOCATE, ASSIGNMENT, IF, WHILE, LIST} kind;

} statement;

typedef struct variable{
    int lineno;
    char *id;
    struct variable *var;
    //TODO Skal være noget mere her
} variable;

typedef struct expression{
    int lineno;
    enum {EXP, TERM} kind;
    struct expression *expression;
    struct term *term;
    //TODO Skal være noget mere her
} expression;

typedef struct term{
    int lineno;
    enum {term_ID, term_PAR, term_EXPRESSION, term_ABS, term_NUM, term_TRUE, term_FALSE, term_NULL} kind;
    union {
        //TODO Skal være noget mere her
    };
} term;

typedef struct act_list{
    int lineno;
    enum {LIST, EMPTY} kind;
    struct exp_list *list;
} act_list;

typedef struct exp_list{
    int lineno;
    enum {EXP, LIST} kind;
    struct expression *expression;
    struct exp_list *list;
} exp_list;



type *make_type_id(char *id);


EXP *makeEXPArithmeticstructure(EXP *left, EXP *right, kind kind);

/*
typedef enum {idK,intconstK,timesK,divK,plusK,minusK} kind;
typedef struct EXP {
  int lineno;
  kind kind;
  union {
    char *idE;
    int intconstE;
    struct {struct EXP *left; struct EXP *right;} timesE;
    struct {struct EXP *left; struct EXP *right;} divE;
    struct {struct EXP *left; struct EXP *right;} plusE;
    struct {struct EXP *left; struct EXP *right;} minusE;
  } val;
} EXP;


 
EXP *makeEXPid(char *id);

EXP *makeEXPintconst(int intconst);

EXP *makeEXPtimes(EXP *left, EXP *right);

EXP *makeEXPdiv(EXP *left, EXP *right);

EXP *makeEXPplus(EXP *left, EXP *right);

EXP *makeEXPminus(EXP *left, EXP *right);
*/
#endif
