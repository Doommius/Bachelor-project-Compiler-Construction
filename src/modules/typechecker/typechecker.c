#include <stdio.h>
#include <stdlib.h>
#include "typechecker.h"
#include "error.h"
#include "symbol.h"

int typecheck(body *program){

    symbol_table *table;
    table = init_symbol_table();

    setup_body(program, table);


}
