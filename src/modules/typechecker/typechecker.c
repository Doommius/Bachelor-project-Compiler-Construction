/**
 * @brief 
 * 
 * @file typechecker.c
 * @author Morten Jæger, Mark Jervelund & Troels Blicher Petersen
 * @date 2018-03-09
 */

#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "check.h"
#include "typechecker.h"
#include "error.h"
#include "symbol.h"
#include "setup.h"
#include "pickup.h"

int typecheck(body *program) {

	symbol_table *table;
	table = init_symbol_table();
#if debugflag > 1
	printf("Starting Setup\n");
#endif
	setup_body(program, table);
#if debugflag > 1
	printf("Starting pickup\n");
#endif
	pickup_body(program);
#if debugflag > 1
	printf("Starting check\n");
#endif
	check_body(program);

	return 0;
}
