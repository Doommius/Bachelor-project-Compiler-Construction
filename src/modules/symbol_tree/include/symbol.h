#ifndef __symbol_h
#define __symbol_h

#define HashSize 317
/* SYMBOL will be extended later.
Function calls will take more parameters later.
*/

typedef struct SYMBOL {
    char *name;
    int value;
    struct SYMBOL *next;
} SYMBOL;

typedef struct symbol_table {
    SYMBOL *table[HashSize];
    struct symbol_table *next;
} symbol_table;

int hash(char *str);

symbol_table *init_symbol_table();

symbol_table *scope_symbol_table(symbol_table *t);

SYMBOL *put_symbol(symbol_table *t, char *name, int value);

SYMBOL *get_symbol(symbol_table *t, char *name);

void dump_symbol_table(symbol_table *t);

SYMBOL *check_local(symbol_table *t, char *name);

void print_symbol(SYMBOL *symbol);

#endif