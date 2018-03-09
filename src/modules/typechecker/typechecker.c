/**
 * @brief 
 * 
 * @file typechecker.c
 * @author Morten Jæger, Mark Jervelund & Troels Blicher Petersen
 * @date 2018-03-09
 */

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
