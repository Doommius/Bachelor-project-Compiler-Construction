// First file!
#include <stdio.h>
#include "symbol.h"
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define testString         "kitty"
#define testString2        "tesu"
#define testString3        "tets"

int main(int argc, char **argv) {
    int returnvalue = 1;
    int testpassed = 0;
    int totaltests = 0;



    printf("Testing hash function\n");
    totaltests++;
    int i = (Hash(testString));
    if (i == 199) {
        printf(ANSI_COLOR_GREEN     "Test %i - Hash function test PASSED\n"     ANSI_COLOR_RESET "\n", totaltests);
        testpassed++;
    } else {
        printf(ANSI_COLOR_RED     "Test %i - Hash functon test FAILED\n"     ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }
    printf("\n");


    printf("Testing init table function\n");
    totaltests++;
    SymbolTable *table = initSymbolTable();
    if (table != NULL) {
        printf(ANSI_COLOR_GREEN     "Test %i - Table constructed PASSED\n"     ANSI_COLOR_RESET "\n", totaltests);
        testpassed++;
    } else {
        printf(ANSI_COLOR_RED     "Test %i - Table construction FAILED\n"     ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }

    printf("\n");
    printf("Testing putsymbol function\n");
    totaltests++;
    SYMBOL *symbol = putSymbol(table, testString, Hash(testString));
    if (symbol != NULL) {
        if (symbol->value == Hash(testString) && (strcmp(symbol->name, testString) == 0)) {
            printf(ANSI_COLOR_GREEN     "Test %i - Symbol correctly made.\n"     ANSI_COLOR_RESET "\n", totaltests);
            testpassed++;
        } else {
            printf(ANSI_COLOR_RED     "Test %i - Symbol creation FAILED\n"     ANSI_COLOR_RESET "\n", totaltests);
            printf("Symbol was made but data was wrong");
            returnvalue = -1;
        }
    } else {
        printf(ANSI_COLOR_RED     "Test %i - Symbol creation FAILED\n"     ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }

    printf("\n");
    printf("Testing getSymbol function\n");
    totaltests++;
    SYMBOL *symbol2 = getSymbol(table, testString);
    if (symbol2 != NULL) {
        if (symbol2->value == Hash(testString) && (strcmp(symbol2->name, testString) == 0)) {
            printf(ANSI_COLOR_GREEN     "Test %i - Symbol correctly made.\n"     ANSI_COLOR_RESET "\n", totaltests);
            testpassed++;
        }
    } else {
        printf(ANSI_COLOR_RED     "Test %i - Symbol creation FAILED\n"     ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }

    printf("\n");
    printf("Testing putsymbol function again\n");
    totaltests++;
    symbol = putSymbol(table, testString, Hash(testString));
    if (symbol != NULL) {
        if (symbol->value == Hash(testString) && (strcmp(symbol->name, testString) == 0)) {
            printf(ANSI_COLOR_GREEN     "Test %i - Symbol correctly made.\n"     ANSI_COLOR_RESET "\n", totaltests);
            testpassed++;
        } else {
            printf(ANSI_COLOR_RED     "Test %i - Symbol creation FAILED\n"     ANSI_COLOR_RESET "\n", totaltests);
            printf("Symbol was made but data was wrong");
            returnvalue = -1;
        }
    } else {
        printf(ANSI_COLOR_RED     "Test %i - Symbol creation FAILED\n"     ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }

    printf("\n");
    printf("Testing putSymbol with two strings that hash to the same, but are different\n");
    totaltests++;
    SYMBOL *symbol3 = putSymbol(table, testString2, Hash(testString2));
    SYMBOL *symbol4 = putSymbol(table, testString3, Hash(testString3));

    if (symbol3 != NULL && symbol4 != NULL) {
        if (symbol3->value == Hash(testString2) && (strcmp(symbol3->name, testString2) == 0)) {
            if (symbol4->value == Hash(testString3) && (strcmp(symbol4->name, testString3) == 0)) {
                printf(ANSI_COLOR_GREEN     "Test %i - Symbol correctly made.\n"     ANSI_COLOR_RESET "\n", totaltests);
                testpassed++;
            }
        } else {
            printf(ANSI_COLOR_RED     "Test %i - Symbol creation FAILED\n"     ANSI_COLOR_RESET "\n", totaltests);
            printf("Symbol was made but data was wrong");
            returnvalue = -1;
        }
    } else {
        printf(ANSI_COLOR_RED     "Test %i - Symbol creation FAILED\n"     ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }

    printf("Testing scopeSymbolTable function\n");
    totaltests++;
    SymbolTable *newTable = scopeSymbolTable(table);
    if (newTable != NULL) {
        printf(ANSI_COLOR_GREEN     "Test %i - Table constructed PASSED\n"     ANSI_COLOR_RESET "\n", totaltests);
        testpassed++;
    } else {
        printf(ANSI_COLOR_RED     "Test %i - Table construction FAILED\n"     ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }

    printf("\n");
    printf("Testing putsymbol function in new table\n");
    totaltests++;
    SYMBOL *symbol5 = putSymbol(newTable, testString, Hash(testString));
    if (symbol5 != NULL) {
        if (symbol5->value == Hash(testString) && (strcmp(symbol5->name, testString) == 0)) {
            printf(ANSI_COLOR_GREEN     "Test %i - Symbol correctly made.\n"     ANSI_COLOR_RESET "\n", totaltests);
            testpassed++;
        } else {
            printf(ANSI_COLOR_RED     "Test %i - Symbol creation FAILED\n"     ANSI_COLOR_RESET "\n", totaltests);
            printf("Symbol was made but data was wrong");
            returnvalue = -1;
        }
    } else {
        printf(ANSI_COLOR_RED     "Test %i - Symbol creation FAILED\n"     ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }


    printf("\n \n");

    printf("tests PASSED %i\n", testpassed);
    printf("\n \n");


    if (totaltests - testpassed != 0) {
        printf(ANSI_COLOR_RED     "test FAILED %i\n"     ANSI_COLOR_RESET "\n", totaltests - testpassed);
    }

    dumpSymbolTable(newTable);

    return returnvalue;

}