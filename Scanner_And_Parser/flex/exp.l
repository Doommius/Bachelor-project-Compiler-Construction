%{
#include "y.tab.h"
#include <string.h>

extern int lineno;
extern int fileno();
%}



%option noyywrap nounput noinput

/* abbreviation of symbols we match on, TO BE EXPANDED */
SYMBOLS [+\-*\/\(\)\[\]{}!\|,\.]


/* Rules
   TODO Add some things that do stuff?



*/
%%
[ \t]+        /* ignore */;
\n              lineno++;

{SYMBOLS}       return yytext[0];

"=="            return EQ;
"!="            return NEQ;
"<"             return LT;
"<="            return LEQ;
">"             return GT;
">="            return GEQ;
"="             return ASSIGN /* maybe not? */ ;
":="            return ASSIGN;
"if"            return IF;
"else"          return ELSE;
"while"         return WHILE;
"for"           return FOR;
"in"            return IN;
";"             return SEMICOLON;
"int"           return TYPEINT;
"string"        return TYPESTRING;
"boolean"       return TYPEBOOLEAN;
"bool"          return TYPEBOOLEAN;
"function"      return FUNCTION;
"return"        return RETURN;
"&&"            return AND;
"and"           return AND;
"||"            return OR;
"or"            return OR;
"true"          return TRUE;
"false"         return FALSE;
"null"          return _NULL;

0|([1-9][0-9]*)        { yylval.intconst = atoi(yytext);
                       return tINTCONST; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.stringconst = (char *)malloc(strlen(yytext)+1);
                         sprintf(yylval.stringconst,"%s",yytext);
                         return tIDENTIFIER; }


.               /* ignore */;

%%
