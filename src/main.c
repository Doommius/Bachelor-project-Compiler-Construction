#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "symbol.h"
#include "tree.h"
#include "pretty.h"
#include "y.tab.h"
#include "weeder.h"
#include "typechecker.h"

int lineno;

body *theprogram;

int main(int argc, char **argv) {
/*
    //    char str[100]; // make sure that this size is enough to hold the single line
    int no_line = 1;

    SymbolTable *table = initSymbolTable();
    //    char str[100]; // make sure that this size is enough to hold the single line
    char buf[100];
    do {
        fgets(buf, sizeof buf, stdin);
        if (buf[strlen(buf) - 1] == '\n') {
            char *str;
            str = malloc((strlen(buf) - 1));
            strcpy(str, buf);
            str[strlen(buf) - 1] = 0;
            SYMBOL *symbol = putSymbol(table, str, Hash(str));
        } else {
            break;
        }
    } while (buf != "\0");

    dumpSymbolTable(table);
*/

 if (argc < 2) {
        freopen("input.txt", "r", stdin);
    } else {
        freopen(argv[1], "r", stdin);
    }

    lineno = 1;
    yyparse();
    //prettyBody(theprogram);
    weeder(theprogram);

    
    printf("\n");
    return 1;
}
