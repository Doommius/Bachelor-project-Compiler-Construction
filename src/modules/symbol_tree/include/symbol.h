#ifndef __symbol_h
#define __symbol_h

#include "tree.h"

#define HashSize 317
/* SYMBOL will be extended later.
Function calls will take more parameters later.
*/

typedef enum { symbol_ID,
               symbol_INT,
               symbol_BOOL,
               symbol_RECORD,
               symbol_ARRAY,
               symbol_NULL,
               symbol_FUNCTION } SYMBOL_type;

typedef struct SYMBOL {
    char *name;
    int value;
    struct SYMBOL *next;
} SYMBOL;

typedef struct SymbolTable {
    SYMBOL *table[HashSize];
    struct SymbolTable *next;
} SymbolTable;


typedef struct symbolType {
    SYMBOL_type type;
    union {
        struct type *array_type;
        struct var_decl_list *record_type;
        struct type *id_type;
        struct {
            struct type *ret_type;
            struct par_decl_list *pdl;
            struct function *func;
        } func_type;
    } val;
} symbolType;

int Hash(char *str);

SymbolTable *initSymbolTable();

SymbolTable *scopeSymbolTable(SymbolTable *t);

SYMBOL *putSymbol(SymbolTable *t, char *name, int value, symbolType *st);

SYMBOL *getSymbol(SymbolTable *t, char *name);

void dumpSymbolTable(SymbolTable *t);

SYMBOL *checkLocal(SymbolTable *t, char *name);

void printSymbol(SYMBOL *symbol);

#endif