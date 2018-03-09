#include <stdio.h>
#include <stdlib.h>
#include "typechecker.h"
#include "error.h"
#include "symbol.h"
#include "setup.h"

int typecheck(body *program){

    symbol_table *table;
    table = init_symbol_table();

    printf("Starting Setup\n");
    setup_body(program, table);

    return 0;
}
