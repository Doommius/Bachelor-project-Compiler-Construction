#ifndef __table_h
#define __table_h

/**
 *
 * Generic hash table
 * Based on the implementation by Andrew W. Appel:
 * http://www.cs.princeton.edu/~appel/modern/c/chap4/table.h
 * http://www.cs.princeton.edu/~appel/modern/c/chap4/table.c 
 * 
 * Don't like that key is a pointer instead of an int
*/

#define TABSIZE 127

typedef struct binder {
	void *key;
	void *value;
	struct binder *next;
	void *prevtop;
} binder;

typedef struct table {
	binder *table[TABSIZE];
	void *top;
} table;

//Make new table
table *table_empty();

//Create binding
binder *table_binder(void *key, void *value, binder *next, void *prevtop);

//Create mappin key->value in table
void table_enter(table *t, void *key, void *value);

//Look up mosst recent binding for key
void *table_look(table *t, void *key);

//Pop most recent binding, and return its key
void *table_pop(table *t);

#endif