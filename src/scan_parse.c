#include <stdio.h>
#include "tree.h"
#include "pretty.h"
#include "y.tab.h"


int lineno;

body *theprogram;

int main() {

    freopen("input.txt", "r", stdin);
    lineno = 1;
    yyparse();
    prettyBody(theprogram);
    printf("\n");
    return 0;
}
