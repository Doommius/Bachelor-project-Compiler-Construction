%{
#include <stdio.h>
#include "tree.h"

extern char *yytext;
extern EXP *theexpression;

void yyerror() {
   printf("syntax error before %s\n",yytext); 
}
%}

%union {
   int intconst;
   char *stringconst;
   struct EXP *exp;
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


%type <exp> program exp

%start program

%left '+' '-'
%left '*' '/'

%% 
program: exp
         { theexpression = $1;}
;

exp : tIDENTIFIER
      {$$ = makeEXPid($1);}
    | tINTCONST
      {$$ = makeEXPintconst($1);}
    | exp '*' exp
      {$$ = makeEXPArithmeticstructure($1,$3,timesK);}
    | exp '/' exp
      {$$ = makeEXPArithmeticstructure($1,$3,divK);}
    | exp '+' exp
      {$$ = makeEXPArithmeticstructure($1,$3,plusK);}
    | exp '-' exp
      {$$ = makeEXPArithmeticstructure($1,$3,minusK);}
    | exp '%' exp
      {$$ = makeEXPArithmeticstructure($1,$3,modK);}
    | '(' exp ')'
      {$$ = $2;}
;

%%
