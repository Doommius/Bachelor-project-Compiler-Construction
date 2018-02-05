int Hash(char *str);

SymbolTable *initSymbolTable();

SymbolTable *scopeSymbolTable(SymbolTable *t);


//Should the new symbol be put in front or the end of the symbol list. also how do we react if the symbol list is full?
SYMBOL *putSymbol(SymbolTable *t, char *name, int value){
// if we assume we just append the list to where ever there is space.
SYMBOL* table = SymbolTable->table;
do{
table[++]
}while (table != NULL)
// i think this should work right?
table->name = name;
table.value = value;
table->next = (table-1)

};

SYMBOL *getSymbol(SymbolTable *t, char *name);

void dumpSymbolTable(SymbolTable *t);
