#include <stddef.h>
#include <stdio.h>
#include <memory.h>
#include "../include/symbol.h"
#include "../include/memory.h"
#include "../include/struct.h"


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
    return k;
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
        table->table[i] = Malloc(sizeof(struct SYMBOL));
        table->table[i]->name = Malloc(sizeof(char)*64);
        table->table[i]->value = 0;
        table->table[i]->next = NULL;

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


//Should the new symbol be put in front or the end of the symbol list. also how do we react if the symbol list is full?
/*
putSymbol takes a hash table and a string, name, as arguments and inserts name into the hash table together with the associated value value. A pointer
to the SYMBOL value which stores name is returned.
*/
SYMBOL *putSymbol(SymbolTable *t, char *name, int value) {
    if( t == NULL ) {
        return NULL;
    }

    int i = 0;
    while( i < HashSize ) {
        if( t->table[i]->next == NULL ) {

            // Allocate memory
            t->table[i]->next = Malloc(sizeof(SYMBOL));

            // Assign values
            t->table[i]->next->name = name;
            t->table[i]->next->value = value;
            t->table[i]->next->next = NULL;

            // Return the SYMBOL
            return t->table[i]->next;
        }
        ++i;
    }

    return NULL;
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


    //Loop over current table.

    int i = 0;
    while (i < HashSize) {
        if (name == t->table[i]->name) {
            return t->table[i];
        }
        i++;
    }
    return getSymbol(t->next, name);

}


/*
 * dumpSymbolTable takes a pointer to a hash table t as argument and prints all
 * the (name, value) pairs that are found in the hash tables from t up to the root.
 * Hash tables are printed one at a time. The printing should be formatted in a nice
 * way and is intended to be used for debugging (of other parts of the compiler).
*/
void dumpSymbolTable(SymbolTable *t) {


    int i = 0;
    while (i < HashSize) {
        printf("%s, %i", t->table[i]->name, t->table[i]->value);
        i++;
    }


    if (t->next == NULL) {
        dumpSymbolTable(t->next);
    }


}
