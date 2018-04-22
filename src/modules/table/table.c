#include <stdio.h>
#include "table.h"
#include "memory.h"

//Make new table
table *table_empty(){
    table *t;
    t = NEW(table);
    t->top = NULL;
    for (int i = 0; i < TABSIZE; i++){
        t->table[i] = NULL;
    }
    return t;
}

//Create binding
binder *table_binder(void *key, void *value, binder *next, void *prevtop){
    binder *b;
    b = NEW(binder);
    b->key = key;
    b->value = value;
    b->next = next;
    b->prevtop = prevtop;
    return b;
}

//Create mappin key->value in table
void table_enter(table *t, void *key, void *value){
    int index;
    index = ((unsigned) key) % TABSIZE;
    t->table[index] = table_binder(key, value, t->table[index], t->top);
    t->top = key;
      
}

//Look up most recent binding for key
void *table_look(table *t, void *key){
    int index;
    binder *b;
    index = ((unsigned) key) % TABSIZE;
    b = t->table[index];
    while (b != NULL){
        if (b->key == key){
            return b->value;
        }
        b = b->next;
    }

    return NULL;

    
    
}

//Pop most recent binding, and return its key
void *table_pop(table *t){
    void *k;
    binder *b;
    int index;
    k = t->top;
    index = ((unsigned) k) & TABSIZE;
    b = t->table[index];
    t->table[index] = b->next;
    t->top = b->prevtop;
    return b->key;
    
}