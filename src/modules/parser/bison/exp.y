
//Comments
// String followed by : is detected as a decleartion evening when within string, code still works, but syntax highlighting is messed up.

%{
#include <stdio.h>
#include "../src/modules/parser/include/tree.h"
#include "y.tab.h"
extern char *yytext;
//extern EXP *theexpression;
extern body *theprogram;
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
%token RETURN
%token AND
%token OR
%token ASSIGN
%token TRUE
%token FALSE
%token _NULL
%token FUNC
%token END
%token INT
%token BOOL
%token ARRAY_OF
%token RECORD_OF
%token TYPE
%token VAR
%token OF_LENGTH
%token THEN
%token WRITE
%token ALLOCATE
%token DO



%type <function> function
%type <head> head
%type <tail> tail
%type <type> type
%type <par_decl_list> par_decl_list
%type <var_decl_list> var_decl_list
%type <var_type> var_type
%type <body> body
%type <decl_list> decl_list
%type <declaration> declaration
%type <statement_list> statement_list
%type <statement> statement
%type <expression> expression
%type <term> term
%type <act_list> act_list
%type <variable> variable
%type <exp_list> exp_list

%start program

%precedence NEG

%left AND '|'
%left EQ NEQ
%left GT LT GEQ LEQ
%left '+' '-'
%left '*' '/'

%%
program:   body
         { theprogram = $1;}
;

function:   head body tail
        {$$ = make_Func($1, $2, $3);
        if (check_Func($1, $3) != 0){
            fprintf(stderr, " Function name : %s, at line %i, does not match function name : %s, at line %i\n ", $1->id, $1->lineno, $3->id, $3->lineno);
            YYABORT;
            }}
;

head:   FUNC tIDENTIFIER '(' par_decl_list ')' ':' type
        {$$ = make_Head($2, $4, $7);}
;

tail:   END tIDENTIFIER
        {$$ = make_Tail($2);}
;

type:   tIDENTIFIER
        {$$ = make_Type_id($1);}
            | INT
        {$$ = make_Type_int();}
            | BOOL
        {$$ = make_Type_bool();}
            | ARRAY_OF type
        {$$ = make_Type_array($2);}
            | RECORD_OF '{' var_decl_list '}'
        {$$ = make_Type_record($3);}
;

par_decl_list:  var_decl_list
        {$$ = make_PDL_list($1);}
            | /*empty*/
        {$$ = make_PDL_empty();}
;

var_decl_list:  var_type ',' var_decl_list
        {$$ = make_VDL_list($1, $3);}
            | var_type
        {$$ = make_VDL_type($1);}
;

var_type:tIDENTIFIER ':' type
        {$$ = make_VType_id($1, $3);}
;

body:decl_list statement_list
        {$$ = make_Body($1, $2);}

;

decl_list:declaration decl_list
        {$$ = make_DL_list($1, $2);}
            | /*empty*/
        {$$ = make_DL_empty();}
;

declaration:TYPE tIDENTIFIER '='     type ';'
        {$$ = make_Decl_type($2, $4);}
            | function
        {$$ = make_Decl_func($1);}
            | VAR var_decl_list ';'
        {$$ = make_Decl_list($2);}
;

statement_list:statement
        {$$ = make_SL_statement($1);}
            | statement statement_list
        {$$ = make_SL_list($1, $2);}
;

statement:RETURN expression ';'
        {$$ = make_STMT_ret($2);}
            | WRITE expression ';'
        {$$ = make_STMT_wrt($2);}
            | ALLOCATE variable ';'
        {$$ = make_STMT_allocate_var($2);}
            | ALLOCATE variable OF_LENGTH expression ';'
        {$$ = make_STMT_allocate_length($2, $4);}
            | variable '=' expression ';'
        {$$ = make_STMT_assign($1, $3);}
            | IF expression THEN statement
        {$$ = make_STMT_if($2, $4);}
            | IF expression THEN statement ELSE statement
        {$$ = make_STMT_if_else($2, $4, $6);}
            | WHILE expression DO statement
        {$$ = make_STMT_while($2, $4);}
            | '{' statement_list '}'
        {$$ = make_STMT_list($2);}
;

variable:tIDENTIFIER
        {$$ = make_Var_id($1);}
            | variable '[' expression ']'
        {$$ = make_Var_exp($1, $3);}
            | variable '.' tIDENTIFIER
        {$$ = make_Var_varid($1, $3);}
;

expression:expression '+' expression
        {$$ = make_EXP(exp_PLUS, $1, $3);}
            | expression '-' expression
        {$$ = make_EXP(exp_MIN, $1, $3);}
            | expression '*' expression
        {$$ = make_EXP(exp_MULT, $1, $3);}
            | expression '/' expression
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
            | expression '|''|' expression
        {$$ = make_EXP(exp_OR, $1, $4);}
            | '-' expression %prec NEG
        {$$ = make_EXP_neg($2);}
            | term
        {$$ = make_EXP_term($1);}
;

term:tINTCONST
        {$$ = make_Term_num($1);}
            | '-' tINTCONST %prec NEG
        {$$ = make_Term_neg_num($2);}
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

act_list:exp_list
        {$$ = make_Act_list($1);}
            | /*empty*/
        {$$ = make_Act_empty();}
;

exp_list:expression
        {$$ = make_ExpL_exp($1);}
            | expression ',' exp_list
        {$$ = make_ExpL_list($1, $3);}
;

%%

