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

typedef struct symbol_table {
    SYMBOL *table[HashSize];
    struct symbol_table *next;
} symbol_table;


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

int hash(char *str);

symbol_table *init_symbol_table();

symbol_table *scope_symbol_table(symbol_table *t);

SYMBOL *put_symbol(symbol_table *t, char *name, int value, symbolType *st);

SYMBOL *get_symbol(symbol_table *t, char *name);

void dump_symbol_table(symbol_table *t);

SYMBOL *check_local(symbol_table *t, char *name);

void print_symbol(SYMBOL *symbol);

#endif