/**
 * @brief Program used for testing different parts of the compiler.
 * 
 * @file symboltabletest.c
 * @author Morten Jæger, Mark Jervelund & Troels Blicher Petersen
 * @date 2018-03-09
 */
#include <stdio.h>
#include "symbol.h"
#include <string.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define testString "kitty"
#define testString2 "tesu"
#define testString3 "tets"


int symboltable_test() {
	int returnvalue = 1;
    int testpassed = 0;
    int totaltests = 0;

    printf("Testing hash function\n");
    totaltests++;
    int i = (hash(testString));
    if (i == 199) {
        printf(ANSI_COLOR_GREEN "Test %i - Hash function test PASSED\n" ANSI_COLOR_RESET "\n", totaltests);
        testpassed++;
    } else {
        printf(ANSI_COLOR_RED "Test %i - Hash functon test FAILED\n" ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }
    printf("\n");

    printf("Testing init table function\n");
    totaltests++;
    symbol_table *table = init_symbol_table();
    if (table != NULL) {
        printf(ANSI_COLOR_GREEN "Test %i - Table constructed PASSED\n" ANSI_COLOR_RESET "\n", totaltests);
        testpassed++;
    } else {
        printf(ANSI_COLOR_RED "Test %i - Table construction FAILED\n" ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }

    printf("\n");
    printf("Testing putsymbol function\n");
    totaltests++;
    SYMBOL *symbol = put_symbol(table, testString, hash(testString), NULL);
    if (symbol != NULL) {
        if (symbol->value == hash(testString) && (strcmp(symbol->name, testString) == 0)) {
            printf(ANSI_COLOR_GREEN "Test %i - Symbol correctly made.\n" ANSI_COLOR_RESET "\n", totaltests);
            testpassed++;
        } else {
            printf(ANSI_COLOR_RED "Test %i - Symbol creation FAILED\n" ANSI_COLOR_RESET "\n", totaltests);
            printf("Symbol was made but data was wrong");
            returnvalue = -1;
        }
    } else {
        printf(ANSI_COLOR_RED "Test %i - Symbol creation FAILED\n" ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }

    printf("\n");
    printf("Testing getSymbol function\n");
    totaltests++;
    SYMBOL *symbol2 = get_symbol(table, testString);
    if (symbol2 != NULL) {
        if (symbol2->value == hash(testString) && (strcmp(symbol2->name, testString) == 0)) {
            printf(ANSI_COLOR_GREEN "Test %i - Symbol correctly returned.\n" ANSI_COLOR_RESET "\n", totaltests);
            testpassed++;
        }
    } else {
        printf(ANSI_COLOR_RED "Test %i - Symbol return FAILED\n" ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }

    printf("\n");
    printf("Testing putsymbol function again\n");
    totaltests++;
    symbol = put_symbol(table, testString, hash(testString), NULL);
    if (symbol != NULL) {
        if (symbol->value == hash(testString) && (strcmp(symbol->name, testString) == 0)) {
            printf(ANSI_COLOR_GREEN "Test %i - Symbol correctly made.\n" ANSI_COLOR_RESET "\n", totaltests);
            testpassed++;
        } else {
            printf(ANSI_COLOR_RED "Test %i - Symbol creation FAILED\n" ANSI_COLOR_RESET "\n", totaltests);
            printf("Symbol was made but data was wrong");
            returnvalue = -1;
        }
    } else {
        printf(ANSI_COLOR_RED "Test %i - Symbol creation FAILED\n" ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }

    printf("\n");
    printf("Testing searching for a symbol, that is not in the table, but the hash for the symbol exists.\n");
    totaltests++;
    SYMBOL *symbol3 = put_symbol(table, testString2, hash(testString2), NULL);

    symbol = get_symbol(table, testString3);

    if (symbol3 != NULL) {
        if (symbol == NULL) {
            printf(ANSI_COLOR_GREEN "Test %i - NULL successfully received.\n" ANSI_COLOR_RESET "\n", totaltests);
            testpassed++;
        }
    } else {
        printf(ANSI_COLOR_RED "Test %i - Symbol search failed\n" ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }

    printf("Testing putSymbol function with a symbol which hash already is in the table\n");
    totaltests++;
    SYMBOL *symbol4 = put_symbol(table, testString3, hash(testString3), NULL);

    if (symbol3 != NULL && symbol4 != NULL) {
        if (symbol3->value == hash(testString2) && (strcmp(symbol3->name, testString2) == 0)) {
            if (symbol4->value == hash(testString3) && (strcmp(symbol4->name, testString3) == 0)) {
                printf(ANSI_COLOR_GREEN "Test %i - Symbol correctly made.\n" ANSI_COLOR_RESET "\n", totaltests);
                testpassed++;
            }
        } else {
            printf(ANSI_COLOR_RED "Test %i - Symbol creation FAILED\n" ANSI_COLOR_RESET "\n", totaltests);
            printf("Symbol was made but data was wrong");
            returnvalue = -1;
        }
    } else {
        printf(ANSI_COLOR_RED "Test %i - Symbol creation FAILED\n" ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }

    printf("Testing scopesymbol_table function\n");
    totaltests++;
    symbol_table *newTable = scope_symbol_table(table);
    if (newTable != NULL) {
        printf(ANSI_COLOR_GREEN "Test %i - Table constructed PASSED\n" ANSI_COLOR_RESET "\n", totaltests);
        testpassed++;
    } else {
        printf(ANSI_COLOR_RED "Test %i - Table construction FAILED\n" ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }

    printf("\n");
    printf("Testing putsymbol function in new table\n");
    totaltests++;
    SYMBOL *symbol5 = put_symbol(newTable, testString, hash(testString), NULL);
    if (symbol5 != NULL) {
        if (symbol5->value == hash(testString) && (strcmp(symbol5->name, testString) == 0)) {
            printf(ANSI_COLOR_GREEN "Test %i - Symbol correctly made.\n" ANSI_COLOR_RESET "\n", totaltests);
            testpassed++;
        } else {
            printf(ANSI_COLOR_RED "Test %i - Symbol creation FAILED\n" ANSI_COLOR_RESET "\n", totaltests);
            printf("Symbol was made but data was wrong");
            returnvalue = -1;
        }
    } else {
        printf(ANSI_COLOR_RED "Test %i - Symbol creation FAILED\n" ANSI_COLOR_RESET "\n", totaltests);
        returnvalue = -1;
    }

    printf("\n \n");

    printf("Tests PASSED %i\n", testpassed);
    printf("\n \n");

    if (totaltests - testpassed != 0) {
        printf(ANSI_COLOR_RED "test FAILED %i\n" ANSI_COLOR_RESET "\n", totaltests - testpassed);
    }

    dump_symbol_table(newTable);

    return returnvalue;
}