%{
#include "y.tab.h"
#include <string.h>

extern int lineno;
extern int fileno();
int nested_comment = 0;

%}

%x COMMENT_SINGLE
%x COMMENT_MULTI

%option noyywrap nounput noinput

/* abbreviation of symbols we match on, TO BE EXPANDED */
SYMBOLS [+\-*\/\(\)\[\]{}\|!,\.=;:]


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
"if"            return IF;
"else"          return ELSE;
"while"         return WHILE;
"return"        return RETURN;
"&&"            return AND;
" || "            return OR;
"true"          return TRUE;
"false"         return FALSE;
"null"          return _NULL;

"func"          return FUNC;
"end"           return END;

"int"           return INT;
"bool"          return BOOL;
"array of"      return ARRAY_OF;
"record of"     return RECORD_OF;
"type"          return TYPE;
"var"           return VAR;
"write"         return WRITE;
"allocate"      return ALLOCATE;
"of length"     return OF_LENGTH;
"then"          return THEN;
"do"            return DO;

0|([1-9][0-9]*)        { yylval.intconst = atoi(yytext);
                       return tINTCONST; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.stringconst = (char *)malloc(strlen(yytext)+1);
                         sprintf(yylval.stringconst,"%s",yytext);
                         return tIDENTIFIER; }

"#"             BEGIN(COMMENT_SINGLE);
"(*"            nested_comment++; BEGIN(COMMENT_MULTI);

<COMMENT_SINGLE>{

\n              lineno++; BEGIN(0);
.               /* ignore */

}

<COMMENT_MULTI>{

\n              lineno++;
"(*"            nested_comment++;
"*)"            {   nested_comment--;
                    if (nested_comment == 0){
                        BEGIN(0);
                    }
                }



.               /* ignore */
<<EOF>>         fprintf(stderr, "Comment not closed at the end of the file. Found at line: %i\n", lineno); exit(1);

}


.               fprintf(stderr, "Unrecognized symbol. Found at line: %i\n", lineno); exit(1);

%%
