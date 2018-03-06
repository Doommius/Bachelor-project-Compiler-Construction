#include <stdio.h>
#include <stdlib.h>
#include "typechecker.h"
#include "error.h"
#include "symbol.h"

int typecheck(body *program){

    SymbolTable *table;
    table = initSymbolTable();

    setup_body(program, table);


}
