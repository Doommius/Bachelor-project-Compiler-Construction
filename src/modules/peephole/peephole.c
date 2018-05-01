#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "tree.h"
#include "code.h"
#include "memory.h"
#include "symbol.h"
#include "peephole.h"



void peephole(a_asm *h){
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
a_asm *remove_move_to_self(a_asm *head){
    struct a_asm *h;
    struct a_asm *current;
    h = head;

    while (head != NULL){
        if (head->ins == MOVQ){
            if (cmp_ops(head->val.two_op.op1, head->val.two_op.op2)){
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

int cmp_ops(asm_op *op1, asm_op *op2){

    //No need to go through switch if they are not the same type
    if (op1->type != op2->type){
        return 0;
    }

    switch(op1->type){

        case (op_TEMP):
            if (op1->val.temp.id == op2->val.temp.id){
                return 1;
            }
            break;

        case (op_REGISTER):
            if (!strcmp(op1->val.reg_id, op2->val.reg_id)){
                return 1;
            }
            break;

        case (op_STACK_LOC):
            return cmp_ops(op1->val.stack.reg, op2->val.stack.reg);
            break;

        case (op_SPILL):
            if (op1 == op2){
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