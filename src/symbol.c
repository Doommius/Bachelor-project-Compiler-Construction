#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "symbol.h"
#include "memory.h"


/*
 * Computes the hash function as seen below.
 *
 */
int Hash(char *str) {
    unsigned int length;
    length = (unsigned) strlen(str);
//    printf("the lenght of the string is %i \n",length);

    int k = (int) str[0];
    int i;
    int pointer = 1;

    while (pointer < length) {
//        printf("shifting k from %i ", k);
        k = k << 1;
//        printf("to %i \n",k);
        i = (int) str[pointer];
//        printf("loaded value i %i \n",i);
//        printf("adding i (%i) and k (%i) \n", i,k);
        k = i + k;
//        printf("sum is %i \n", k);

        pointer++;
    }
    return (k % HashSize);
}

/*
initSymbolTable returns a pointer to a new initialized hash table (of type
        SymbolTable)
*/
SymbolTable *initSymbolTable() {

    int i = 0;
    SymbolTable *table = Malloc(sizeof(SYMBOL) * HashSize);
    table->next = NULL;
    while (i < HashSize) {
        table->table[i] = NULL;
        i++;
    }
    return table;
}

/*
 *
 * scopeSymbolTable takes a pointer to a hash table t as argument and returns
 * a new hash table with a pointer to t in its next field.
 */
SymbolTable *scopeSymbolTable(SymbolTable *t) {
    SymbolTable *newTable = initSymbolTable();
    newTable->next = t;
    return newTable;
}



/*
 * putSymbol takes a hash table and a string, name, as arguments and inserts name into the hash table together with the associated value value. A pointer
 * to the SYMBOL value which stores name is returned.
*/
SYMBOL *putSymbol(SymbolTable *t, char *name, int value) {
    if( t == NULL ) {
        return NULL;
    }
    SYMBOL *localCheck = checkLocal(t, name);
    //Symbol already exists
    if (localCheck != NULL){
        return localCheck;
    }
    else {

        SYMBOL *symbol = Malloc(sizeof(SYMBOL));
        symbol->name = name;
        symbol->value = value;
        symbol->next = Malloc(sizeof(SYMBOL));

        //Placed in front of the list
        symbol->next = t->table[value];
        t->table[value] = symbol;
        return symbol;

    }
}


/*
    getSymbol takes a hash table and a string name as arguments and searches for
    name in the following manner: First search for name in the hash table which
    is one of the arguments of the function call. If name is not there, continue the
    search in the next hash table. This process is repeatedly recursively. If name has
    not been found after the root of the tree (see Fig. 1) has been checked, the result
    NULL is returned. If name is found, return a pointer to the SYMBOL value in
    which name is stored
    */
SYMBOL *getSymbol(SymbolTable *t, char *name) {
//    First check if t is null
    if (t == NULL) {
        return NULL;
    }

    SYMBOL *localCheck = checkLocal(t, name);

    //Symbol in local table?
    if (localCheck != NULL){
        return localCheck;
    }

    if (t->next != NULL){
        getSymbol(t->next, name);
    }

    //Symbol does not exists, should be an error message or such, later in the project
    return NULL;
}


/*
 * dumpSymbolTable takes a pointer to a hash table t as argument and prints all
 * the (name, value) pairs that are found in the hash tables from t up to the root.
 * Hash tables are printed one at a time. The printing should be formatted in a nice
 * way and is intended to be used for debugging (of other parts of the compiler).
*/
void dumpSymbolTable(SymbolTable *t) {
    if (t == NULL){
        return;
    }

    printf("\t\tPrinting symbol table:\n\n");

    for (int i = 0; i < HashSize; i++) {
        SYMBOL *symbol = t->table[i];
        if (symbol != NULL){
            printf("Hash: \t%i ", i);
            while (symbol != NULL){
                printSymbol(symbol);
                printf(" ");
                symbol = symbol->next;
            }
            printf("\n");
        }
    }
    printf("\n");

    dumpSymbolTable(t->next);

}

/*
 * Check the current table we are in for a value
 */
SYMBOL *checkLocal(SymbolTable *t, char *name){
    int hashValue = Hash(name);
    SYMBOL *symbol = t->table[hashValue];
    if (symbol == NULL){
        return NULL;
    }

    else {
        while (symbol != NULL){
            if (strcmp(name, symbol->name) == 0){
                return symbol;
            }
            symbol = symbol->next;
        }
    }
}

void printSymbol(SYMBOL *symbol){
    printf("(%s, %i)", symbol->name, symbol->value);
}