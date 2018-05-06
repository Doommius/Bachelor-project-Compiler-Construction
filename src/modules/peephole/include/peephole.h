#ifndef __peephole_h
#define __peephole_h
#include "code.h"

void peephole(a_asm *head);

a_asm *remove_move_to_self(a_asm *head);

a_asm *remove_mem_moves(a_asm *head);

int cmp_ops(asm_op *op1, asm_op *op2);


#endif
