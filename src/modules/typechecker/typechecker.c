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
#include "pickup.h"

int typecheck(body *program){

    symbol_table *table;
    table = init_symbol_table();

    printf("Starting Setup\n");
    setup_body(program, table);

    printf("Starting pickup\n");
    pickup_body(program);

    printf("Starting check\n");
    check_body(program);

    return 0;
}
