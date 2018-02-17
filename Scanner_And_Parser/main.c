#include <stdio.h>
#include "tree.h"
#include "pretty.h"
#include "y.tab.h"


int lineno;

expression *theprogram;

int main() {

    freopen("input.txt", "r", stdin);
    lineno = 1;
    yyparse();
    prettyEXP(theprogram);
    printf("\n");
    return 0;
}
