//
// Created by jervelund on 2/9/18.
//
#include "symbol.h"
#include <string.h>
#include <stdio.h>
#include <struct.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char **argv) {

//    char str[100]; // make sure that this size is enough to hold the single line
    int no_line = 1;

    SymbolTable *table = initSymbolTable();
    char buf[100];
    do {
        fgets(buf, sizeof buf, stdin);
        if (buf[strlen(buf) - 1] == '\n') {
            char *str;
            str = malloc((strlen(buf)-1));
            strcpy(str,buf);
            str[strlen(buf)-1] = 0;
            SYMBOL *symbol = putSymbol(table, str, Hash(str));
//            printSymbol(symbol);
        } else {
        break;
        }
//        dumpSymbolTable(table);
    } while (buf != "\0");

    dumpSymbolTable(table);

    return 1;


}