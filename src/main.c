// First file!
#include <stdio.h>
#include "../include/symbol.h"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"




int main(int argc, char **argv){
    int returnvalue = 1;
    int testpassed = 0;
    int totaltests = 0;



    printf("Testing hash function\n");
    totaltests++;
    int i =(Hash("kitty"));
    if (i == 3369){
        printf(ANSI_COLOR_GREEN     "Test 1 - Hash function test PASSED\n"     ANSI_COLOR_RESET "\n");
        testpassed++;
    }else{
        printf(ANSI_COLOR_RED     "Test 1 - Hash functon test FAILED\n"     ANSI_COLOR_RESET "\n");
        returnvalue = -1;
    }
    printf("\n");


    printf("Testing init table function\n");
    totaltests++;
    SymbolTable *table = initSymbolTable();
    if (table != NULL){
        printf(ANSI_COLOR_GREEN     "Test 2 - Table constructed PASSED\n"     ANSI_COLOR_RESET "\n");
        testpassed++;
    }else{
        printf(ANSI_COLOR_RED     "Test 2 - Table construction FAILED\n"     ANSI_COLOR_RESET "\n");
        returnvalue = -1;
    }

    printf("\n");
    printf("Testing putsymbol function\n");
    totaltests++;
    SYMBOL *symbol = putSymbol(table, "kitty", Hash("kitty"));
    if (symbol->value == Hash("kitty") && symbol->name == "kitty"){
        printf(ANSI_COLOR_GREEN     "Test 3 - Symbol correctly made.\n"     ANSI_COLOR_RESET "\n");
        testpassed++;
    }else{
        printf(ANSI_COLOR_RED     "Test 3 - Symbol creation FAILED\n"     ANSI_COLOR_RESET "\n");
        returnvalue = -1;
    }




    printf("\n \n");

    printf("tests PASSED %i\n",testpassed);
    if (totaltests-testpassed != 0){
        printf(ANSI_COLOR_RED     "test FAILED %i\n"     ANSI_COLOR_RESET "\n", totaltests-testpassed);
    }


    return returnvalue;

}