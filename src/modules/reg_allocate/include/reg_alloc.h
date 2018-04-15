#ifndef __reg_alloc_h
#define __reg_alloc_h

#include "code.h"

#define AVAIL_REGS 14

a_asm *reg_alloc(a_asm *h, int local_vars);

void allocate(asm_op **op);

int get_reg(asm_op *op);


#endif