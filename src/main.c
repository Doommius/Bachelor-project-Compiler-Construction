#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "struct.h"
#include "symbol.h"

int main(int argc, char **argv) {


    int no_line = 1;

    SymbolTable *table = initSymbolTable();
    //    char str[100]; // make sure that this size is enough to hold the single line
    char buf[100];
    do {
        fgets(buf, sizeof buf, stdin);
        if (buf[strlen(buf) - 1] == '\n') {
            char *str;
            str = malloc((strlen(buf)-1));
            strcpy(str,buf);
            str[strlen(buf)-1] = 0;
            SYMBOL *symbol = putSymbol(table, str, Hash(str));
        } else {
            break;
        }
    } while (buf != "\0");

    dumpSymbolTable(table);

    return 1;


}
