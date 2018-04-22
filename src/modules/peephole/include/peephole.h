#ifndef __peephole_h
#define __peephole_h
#include "code.h"

void peephole(a_asm *head);

a_asm *remove_move_to_self(a_asm *head);

a_asm *fold_moves(a_asm *head);

int cmp_ops(asm_op *op1, asm_op *op2);

a_asm *get_last_use(a_asm *ins, asm_op *op);

int ever_used(a_asm *from, a_asm *to, asm_op *target);

a_asm *remove_indirect_move(a_asm *head);

#endif
