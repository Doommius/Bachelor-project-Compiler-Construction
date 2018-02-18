%{
#include <stdio.h>
#include "tree.h"
#include "y.tab.h"


extern char *yytext;
//extern EXP *theexpression;
extern expression *theprogram;

void yyerror() {
   printf("syntax error before %s\n",yytext);
}
%}

%union {
    int intconst;
    char *stringconst;
    struct EXP *exp;
    struct function *function;
    struct head *head;
    struct tail *tail;
    struct type *type;
    struct par_decl_list *par_decl_list;
    struct var_decl_list *var_decl_list;
    struct var_type *var_type;
    struct body *body;
    struct decl_list *decl_list;
    struct declaration *declaration;
    struct statement_list *statement_list;
    struct statement *statement;
    struct variable *variable;
    struct expression *expression;
    struct term *term;
    struct act_list *act_list;
    struct exp_list *exp_list;
}

%token <intconst> tINTCONST
%token <stringconst> tIDENTIFIER
%token EQ
%token NEQ
%token LEQ
%token GEQ
%token LT
%token GT
%token IF
%token ELSE
%token WHILE
%token FOR
%token IN
%token FUNCTION
%token TYPEINT
%token TYPESTRING
%token TYPEBOOLEAN
%token SEMICOLON
%token RETURN
%token AND
%token OR
%token ASSIGN
%token TRUE
%token FALSE
%token _NULL

%type <expression> expression
%type <term> term
%type <act_list> act_list
%type <variable> variable
%type <exp_list> exp_list

%start program

%left AND OR
%left EQ NEQ
%left GT LT GEQ LEQ
%left '+' '-'
%left '*' '/'

%%
program :   expression
         { theprogram = $1;}
;

variable    :   tIDENTIFIER
        {$$ = make_Var_id($1);}
            | variable '[' expression ']'
        {$$ = make_Var_exp($1, $3);}
            | variable '.' tIDENTIFIER
        {$$ = make_Var_varid($1, $3);}

expression  :   expression '+' expression
        {$$ = make_EXP(exp_PLUS, $1, $3);}
            | expression '-' expression
        {$$ = make_EXP(exp_MIN, $1, $3);}
            | expression '*' expression
        {$$ = make_EXP(exp_MULT, $1, $3);}
            | expression '/' expressiona.list
        {$$ = make_EXP(exp_DIV, $1, $3);}
            | '(' expression ')'
        {$$ = $2;}
            | expression EQ expression
        {$$ = make_EXP(exp_EQ, $1, $3);}
            | expression NEQ expression
        {$$ = make_EXP(exp_NEQ, $1, $3);}
            | expression GT expression
        {$$ = make_EXP(exp_GT, $1, $3);}
            | expression LT expression
        {$$ = make_EXP(exp_LT, $1, $3);}
            | expression GEQ expression
        {$$ = make_EXP(exp_GEQ, $1, $3);}
            | expression LEQ expression
        {$$ = make_EXP(exp_LEQ, $1, $3);}
            | expression AND expression
        {$$ = make_EXP(exp_AND, $1, $3);}
            | expression OR expression
        {$$ = make_EXP(exp_OR, $1, $3);}
            | term
        {$$ = make_EXP_term($1);}
;

term    :       tINTCONST
        {$$ = make_Term_num($1);}
            | '(' expression ')'
        {$$ = make_Term_par($2);}
            | '!' term
        {$$ = make_Term_not($2);}
            | '|' expression '|'
        {$$ = make_Term_abs($2);}
            | TRUE
        {$$ = make_Term_boolean(1);}
            | FALSE
        {$$ = make_Term_boolean(0);}
            | _NULL
        {$$ = make_Term_null();}
            | variable
        {$$ = make_Term_variable($1);}
            | tIDENTIFIER '(' act_list ')'
        {$$ = make_Term_list($1, $3);}
;

act_list:       exp_list
        {$$ = make_Act_list($1);}
            | /*empty*/
        {$$ = make_Act_empty();}
;

exp_list:       expression
        {$$ = make_ExpL_exp($1);}
            | expression ',' exp_list
        {$$ = make_ExpL_list($1, $3);}
;

%%
