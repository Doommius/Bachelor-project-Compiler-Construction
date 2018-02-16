%{
#include "y.tab.h"
#include <string.h>

extern int lineno;
extern int fileno();
%}

%option noyywrap nounput noinput

SYMBOLS [+\-]

%%
[ \t]+        /* ignore */;
\n              lineno++;


[a-zA-Z_][a-zA-Z0-9_]* { yylval.stringconst = (char *)malloc(strlen(yytext)+1);
                         sprintf(yylval.stringconst,"%s",yytext);
                         return tIDENTIFIER; }



/*
"*"             return '*';
"/"             return '/';
"+"             return '+';
"-"             return '-';
"("             return '(';
")"             return ')';


/* start added by mark */
"="             return EQ;
"!="            return NEG;
"<"             return LT;
"<="            return LE;
">"             return GT;
">="            return GE;
"if"            return IF;
"else"          return ELSE;
"while"         return WHILE;
"for"           return FOR;
"in"            return IN;
"{"             return '{';
"}"             return '}';
";"             return SEMICOLON;



/* end added by mark */

0|([1-9][0-9]*)        { yylval.intconst = atoi(yytext);
                       return tINTCONST; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.stringconst = (char *)malloc(strlen(yytext)+1);
                         sprintf(yylval.stringconst,"%s",yytext);
                         return tIDENTIFIER; }

                        */
.               /* ignore */;

%%
