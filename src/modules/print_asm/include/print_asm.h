#ifndef __print_asm_h
#define __print_asm_h

#include "code.h"
#include "linked_list.h"

int print_asm(a_asm *head, char *file);

void create_asm(a_asm *head);

void create_ins(char *type, char *args, char *comment);

void create_label(char *label, char *comment);

void create_comment(char *comment, int offset);

void create_1_op(char *ins, asm_op *op, char *comment);

void create_2_op(char *ins, asm_op *op1, asm_op *op2, char *comment);

void get_opt(asm_op *op, char *dest);



#endif