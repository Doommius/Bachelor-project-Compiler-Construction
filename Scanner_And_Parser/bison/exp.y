%{
#include <stdio.h>
#include "tree.h"

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
    struct acl_list *acl_list;
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

%type <expression> program expression
%type <term> term

%start program

%left '+' '-'
%left '*' '/'

%%
program :   expression
         { theprogram = $1;}
;

expression  :   expression '+' expression
        { $$ = make_EXP(exp_PLUS, $1, $3);}
            | term
        { $$ = make_EXP_term($1);}
;

term    : tINTCONST
        { $$ = make_Term_num($1);}
;


%%
