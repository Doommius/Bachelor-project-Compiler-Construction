#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "tree.h"
#include "code.h"
#include "memory.h"
#include "symbol.h"
#include "peephole.h"

void peephole(a_asm *h) {
	a_asm *head;

	a_asm *prev;
	a_asm *next;
	a_asm *current;

	head = h;

	//For now we just remove MOV temp, temp instructions, where the two temps are the same

	h = remove_move_to_self(h);
}

/**
 * 
 * Removing the following patterns:
 * MOVQ temp1 temp1
 * 
 */
a_asm *remove_move_to_self(a_asm *head) {
	struct a_asm *h;
	struct a_asm *current;
	h = head;

	while (head != NULL) {
		if (head->ins == MOVQ) {
			if (cmp_ops(head->val.two_op.op1, head->val.two_op.op2)) {
				head = head->next;
				current = head->prev;
				current->prev->next = head;
				current->next->prev = current->prev;
				free(current);
			} else {
				head = head->next;
			}

		} else {
			head = head->next;
		}
	}

	return h;
}

/**
 * 
 * Removing the following patterns:
 * MOVQ MEM(something), MEM(something)
 * 
 * Replace with:
 * MOVQ MEM(something), temp1
 * MOVQ temp1, MEM(something)
 * 
 */
a_asm *remove_mem_moves(a_asm *head) {

	struct a_asm *h;
	struct a_asm *replacer;
	struct asm_op *intermediate;
	struct asm_op *old_MEM;

	h = head;
	while (head != NULL) {
		if (head->ins == MOVQ) {
			if ((head->val.two_op.op1->type == op_MEM_LOC || head->val.two_op.op1->type == op_MEM_LOC) && (head->val.two_op.op1->type == op_MEM_LOC || head->val.two_op.op1->type == op_MEM_LOC)) {

				replacer = NEW(a_asm);
				replacer->ins = MOVQ;

				intermediate = make_op_temp();
				old_MEM = head->val.two_op.op2;

				head->val.two_op.op2 = intermediate;

				replacer->ops = 2;
				replacer->val.two_op.op1 = intermediate;
				replacer->val.two_op.op2 = old_MEM;

				replacer->prev = head;
				replacer->next = head->next;

				head->next->prev = replacer;
				head->next = replacer;
			}
		}

		head = head->next;
	}

	return h;
}

int cmp_ops(asm_op *op1, asm_op *op2) {

	//No need to go through switch if they are not the same type
	if (op1->type != op2->type) {
		return 0;
	}

	switch (op1->type) {

	case (op_TEMP):
		if (op1->val.temp.id == op2->val.temp.id) {
			return 1;
		}
		break;

	case (op_REGISTER):
		if (!strcmp(op1->val.reg_id, op2->val.reg_id)) {
			return 1;
		}
		break;

	case (op_STACK_LOC):
		return cmp_ops(op1->val.stack.reg, op2->val.stack.reg);
		break;

	case (op_MEM_LOC):
		return cmp_ops(op1->val.mem_index_reg, op2->val.mem_index_reg);
		break;

	case (op_LEA):
		return cmp_ops(op1->val.lea.reg, op2->val.lea.reg);
		break;

	case (op_SPILL):
		if (op1 == op2) {
			return 1;
		} else {
			return 0;
		}
		break;

	default:
		break;
	}

	return 0;
}