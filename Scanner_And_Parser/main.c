#include <stdio.h>
#include "tree.h"
#include "pretty.h"
#include "y.tab.h"


int lineno;

EXP *theexpression;

int main() {

    freopen("input.txt", "r", stdin);
    lineno = 1;
    yyparse();
    prettyEXP(theexpression);
    printf("\n");
    return 0;
}
