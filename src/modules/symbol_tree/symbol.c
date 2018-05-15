/**
 * @brief Symbol table using hashing.
 * 
 * @file symbol.c
 * @author Morten Jæger, Mark Jervelund & Troels Blicher Petersen
 * @date 2018-03-09
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "symbol.h"
#include "memory.h"

/**
 * @brief Computes the hash function as seen below.
 * 
 * @param str 
 * @return int 
 */
int hash(char *str) {
	int length;
	length = (unsigned)strlen(str);
	int k = (int)str[0];
	int i;
	int pointer = 1;

	while (pointer < length) {
		k = k << 1;
		i = (int)str[pointer];
		k = i + k;
		pointer++;
	}
	return (k % HashSize);
}

/**
 * @brief Initializes the symbol table
 * 
 * @return symbol_table* Returns a pointer to a new initialized hash table (of 
 * type SymbolTable)
 */
symbol_table *init_symbol_table() {

	int i = 0;
	symbol_table *table = Malloc(sizeof(SYMBOL) * HashSize);
	table->next = NULL;
	while (i < HashSize) {
		table->table[i] = NULL;
		i++;
	}
	return table;
}

/**
 * @brief 
 * 
 * @param t Pointer to a hash table
 * @return symbol_table* Returns a new hash table pointing to t.
 */
symbol_table *scope_symbol_table(symbol_table *t) {
	symbol_table *newTable = init_symbol_table();
	newTable->next = t;
	return newTable;
}

/**
 * @brief put_symbol takes a hash table and a string, name, as arguments and 
 * inserts name into the hash table together with the associated value value. 
 * A pointer to the SYMBOL value which stores name is returned.
 * 
 * @param t Pointer to hash table.
 * @param name 
 * @param value 
 * @param st 
 * @return SYMBOL* 
 */
SYMBOL *put_symbol(symbol_table *t, char *name, int value, symbol_type *st) {
	if (t == NULL) {
		return NULL;
	}
	SYMBOL *localCheck = check_local(t, name);
	//Symbol already exists
	if (localCheck != NULL) {
		return localCheck;
	} else {
		int hashValue = hash(name);
		
		SYMBOL *symbol = Malloc(sizeof(SYMBOL));
		symbol->name = name;
		symbol->value = value;
		symbol->stype = st;
		symbol->next = Malloc(sizeof(SYMBOL));

		//Placed in front of the list
		symbol->next = t->table[hashValue];
		t->table[hashValue] = symbol;
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
SYMBOL *get_symbol(symbol_table *t, char *name) {
	//    First check if t is null
	if (t == NULL) {
		return NULL;
	}

	SYMBOL *localCheck = check_local(t, name);

	//Symbol in local table?
	if (localCheck != NULL) {
		return localCheck;
	}

	if (t->next != NULL) {
		return get_symbol(t->next, name);
	}

	//Symbol does not exists
	return NULL;
}

/*
 * dumpSymbolTable takes a pointer to a hash table t as argument and prints all
 * the (name, value) pairs that are found in the hash tables from t up to the root.
 * Hash tables are printed one at a time. The printing should be formatted in a nice
 * way and is intended to be used for debugging (of other parts of the compiler).
*/
void dump_symbol_table(symbol_table *t) {
	if (t == NULL) {
		return;
	}

	printf("Printing symbol table:\n\n");

	for (int i = 0; i < HashSize; i++) {
		if (t->table[i] != NULL) {
			print_symbol(t->table[i]);
			printf("\n");
		}
	}
	printf("\n");

	dump_symbol_table(t->next);
}

/*
 * Check the current table we are in for a value
 */
SYMBOL *check_local(symbol_table *t, char *name) {
	int hashValue = hash(name);

	SYMBOL *symbol = t->table[hashValue];
	if (symbol == NULL) {
		return NULL;
	} else {
		while (symbol != NULL) {
			if (strcmp(name, symbol->name) == 0) {
				return symbol;
			}
			symbol = symbol->next;
		}
	}

	//Hash value for the symbol exists, but the symbol is not in the table
	return NULL;
}

void print_symbol(SYMBOL *symbol) {
	printf("(%s, %i)", symbol->name, symbol->value);
}

/**
 * Calculate depth of a symbol, used for static linking
 */
int get_symbol_depth(symbol_table *t, char *name) {
	struct symbol_table *temp;
	struct SYMBOL *s;
	int depth;
	temp = t;
	depth = 0;

	s = check_local(t, name);

	while (s == NULL) {
		temp = temp->next;
		s = check_local(temp, name);
		depth++;
	}

	return depth;
}

//Returns size of symboltable - used when allocating memory for records
int table_size(symbol_table *t) {
	int size;
	SYMBOL *s;
	size = 0;

	for (int i = 0; i < HashSize; i++) {
		s = t->table[i];
		while (s != NULL) {
			size++;
			s = s->next;
		}
	}

	return size;
}

//Adds a offset to the symbols in the symmbol table - used in records
void create_offset(symbol_table *t) {
	int offset;
	SYMBOL *s;
	offset = 0;

	for (int i = 0; i < HashSize; i++) {
		s = t->table[i];
		while (s != NULL) {
			s->offset = offset;
			s = s->next;
			offset++;
		}
	}
}