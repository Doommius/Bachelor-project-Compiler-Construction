//
// Created by jervelund on 2/9/18.
//
#include <stdio.h>
#include "symbol.h"
#include <string.h>

int main(int argc, char **argv) {

    char str[100]; // make sure that this size is enough to hold the single line
    int no_line=1;

    SymbolTable *table = initSymbolTable();
    while(gets(str) != NULL && strcmp(str,"END"))
    {
        putSymbol(table, str, Hash(str));

    }


    dumpSymbolTable(table);

    return 1;


}