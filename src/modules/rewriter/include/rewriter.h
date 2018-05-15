#ifndef __rewriter_h
#define __rewriter_h

#include "code.h"

a_asm *final_rewrite(int *colors, a_asm *program);

void rewrite_op(int *colors, asm_op **op, int *stack_offset);

//Returns the register corresponding to the given number
asm_op *get_corresponding_reg(int reg);

a_asm *rewrite_function(a_asm *func_start, a_asm *func_end, int offset);

a_asm *add_prefix(int offset);

a_asm *add_postfix(int offset);

a_asm *add_pop_of_live(a_asm *program, int *colors, a_asm *call);

a_asm *add_push_of_live(a_asm *program, int *colors, a_asm *call);

int *get_used_regs(a_asm *program, char *function, int *colors);

#endif