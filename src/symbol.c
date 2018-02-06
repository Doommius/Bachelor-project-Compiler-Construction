#include "../include/symbol.h"


/*
 * Computes the hash function as seen below.
 *
 */
int Hash(char *str) {

    int k = (int) str;
    int i;
    //TODO is this the correct way to loop over the whole string? as we're not given a length.
    while (str != null) {
        k = k >> 1;
        str++;
        i = (int) str;
        k = i + k;
    }
    return k;


}
/*
initSymbolTable returns a pointer to a new initialized hash table (of type
        SymbolTable)
*/
SymbolTable *initSymbolTable() {
    SymbolTable *table = malloc(sizeof(SYMBOL) * HashSize);
    return table;
}
/*
 *
 * scopeSymbolTable takes a pointer to a hash table t as argument and returns
 * a new hash table with a pointer to t in its next field.
 */
SymbolTable *scopeSymbolTable(SymbolTable *t);


//Should the new symbol be put in front or the end of the symbol list. also how do we react if the symbol list is full?
/*
putSymbol takes a hash table and a string, name, as arguments and inserts
        name into the hash table together with the associated value value. A pointer
to the SYMBOL value which stores name is returned.
*/



SYMBOL *putSymbol(SymbolTable *t, char *name, int value) {

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
    if (t == nullptr) {
        return null;
    }

    //Loop over current table.
    SYMBOL *table = SymbolTable;
    i = 0;
    while (i < HashSize) {
        if (table->name == name) {
            return table;
        }
        table->next;
        i++;
    }
    return *getSymbol(t->next, name)

}


/*
 * dumpSymbolTable takes a pointer to a hash table t as argument and prints all
 * the (name, value) pairs that are found in the hash tables from t up to the root.
 * Hash tables are printed one at a time. The printing should be formatted in a nice
 * way and is intended to be used for debugging (of other parts of the compiler).
*/
void dumpSymbolTable(SymbolTable *t);
