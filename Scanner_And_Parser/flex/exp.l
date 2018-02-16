%{
#include "y.tab.h"
#include <string.h>

extern int lineno;
extern int fileno();
%}



%option noyywrap nounput noinput

SYMBOLS [+\-]


/* Rules
   TODO Add some things that do stuff?



*/
%%
[ \t]+        /* ignore */;
\n              lineno++;


[a-zA-Z_][a-zA-Z0-9_]* { yylval.stringconst = (char *)malloc(strlen(yytext)+1);
                         sprintf(yylval.stringconst,"%s",yytext);
                         return tIDENTIFIER; }




"*"             return '*';
"/"             return '/';
"+"             return '+';
"-"             return '-';
"%"             return 'MOD';
"("             return '(';
")"             return ')';
"="             return EQ;
"!="            return NEQ;
"<"             return LT;
"<="            return LEQ;
">"             return GT;
">="            return GEQ;
"if"            return IF;
"else"          return ELSE;
"while"         return WHILE;
"for"           return FOR;
"in"            return IN;
"{"             return '{';
"}"             return '}';
";"             return SEMICOLON;
"int"           return TYPEINT;
"string"        return TYPESTRING;
"boolean"       return TYPEBOOLEAN;
"bool"          return TYPEBOOLEAN;
"function"      return FUNCTION;

0|([1-9][0-9]*)        { yylval.intconst = atoi(yytext);
                       return tINTCONST; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.stringconst = (char *)malloc(strlen(yytext)+1);
                         sprintf(yylval.stringconst,"%s",yytext);
                         return tIDENTIFIER; }


.               /* ignore */;

%%
