#include "tree.h"
#include "pretty.h"
#include "y.tab.h"
#include "lex.yy.c"


int lineno;

void yyparse();

EXP *theexpression;

int main()
{ lineno = 1;
  yyparse();
  prettyEXP(theexpression);
  return 0;
}
