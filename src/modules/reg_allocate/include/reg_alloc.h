#ifndef __reg_alloc_h
#define __reg_alloc_h

#include "linked_list.h"

#define AVAIL_REGS 10

void *liveness_analysis(a_asm program);

a_asm *reg_alloc(a_asm *h, int local_vars);

void allocate(asm_op **op);


#endif