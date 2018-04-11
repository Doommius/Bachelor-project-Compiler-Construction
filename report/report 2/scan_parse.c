#include <stdio.h>
#include "tree.h"
#include "pretty.h"
#include "y.tab.h"


int lineno;

body *theprogram;

int main(int argc, char **argv) {

    if(argc < 2) {
        freopen("input.txt", "r", stdin);
    } else {
        freopen(argv[1], "r", stdin);
    }

    lineno = 1;
    yyparse();
    prettyBody(theprogram);
    printf("\n");
    return 0;

}
