#ifndef __rewriter_h
#define __rewriter_h

#include "code.h"

a_asm *final_rewrite(int *colors, a_asm *program);

void rewrite_op(int *colors, asm_op **op);

//Returns the register corresponding to the given number
asm_op *get_corresponding_reg(int reg);


#endif