// First file!
#include <stdio.h>
#include "../include/symbol.h"

int main(int argc, char **argv){
    int returnvalue = 1;
    int testpassed = 0;



    printf("Testing hash function\n");
    int i =(Hash("kitty"));
    if (i == 3369){
        printf("Test 1 - Hash function test PASSED\n");
        testpassed++;
    }else{
        printf("Test 1 - Hash functon test FAILED\n");
        returnvalue = -1;
    }
    printf("\n \n \n");
    printf("Testing init table function\n");
    SymbolTable *table = initSymbolTable();
    if (table != NULL){
        printf("Test 2 - Table constructed PASSED\n");
        testpassed++;
    }else{
        printf("Test 2 - Table construction FAILED\n");
        returnvalue = -1;
    }






    printf("tests passed %i",testpassed);
    return returnvalue;

}