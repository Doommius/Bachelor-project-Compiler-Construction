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
    h = fold_moves(h);
    h = remove_indirect_move(h);

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

/**
 * 
 * Removing the following patterns:
 * MOVQ something, temp1
 * ...
 * ...
 * MOVQ temp1, temp2
 * 
 * Where temp1 is not used elsewhere, and temp2 is not used between those instructions
 * 
 */
a_asm *fold_moves(a_asm *head){
    struct a_asm *h;
    struct asm_op *target;
    struct a_asm *last_use;

    h = head;

    while (h != NULL){
        if (h->ins == MOVQ){
            if (h->val.two_op.op1->type == op_TEMP && h->val.two_op.op2->type == op_TEMP){
                target = h->val.two_op.op1;
                last_use = get_last_use(h->prev, target);
                
                if (last_use != NULL){
                    if (last_use->ins == MOVQ && cmp_ops(last_use->val.two_op.op2, target)){
                        //Check if target is used from the start of the program, to the instruction "h"
                        if (!ever_used(head, last_use, target) && !ever_used(last_use->next, h, target)){
                            if (!ever_used(h->new, NULL, target)){
                                last_use->val.two_op.op2 = h->val.two_op.op2;
                                h->prev->next = h->next;
                                h->next->prev = h->prev;

                            }
                            
                        }
                    }
                }

            }
        }
        h = h->next;
    }


}

/**
 * 
 * Removing the following patterns:
 * MOVQ temp1, temp2
 * MOVQ temp2, temp3
 * 
 * And replace them by:
 * MOVQ temp1, temp2
 * MOVQ temp1, temp3
 * 
 */
a_asm *remove_indirect_move(a_asm *head){

    struct a_asm *h;
    h = head;

    while (h != NULL){
        if (h->prev != NULL){
            if (h->prev->ins == MOVQ && h->prev->val.two_op.op2->type == op_TEMP){

                if (h->ops == 2 && cmp_ops(h->prev->val.two_op.op2, h->val.two_op.op1)){
                    if (h->val.two_op.op2->type == op_TEMP || h->val.two_op.op2->type == op_REGISTER){
                        if (h->prev->val.two_op.op1->type == op_TEMP || h->prev->val.two_op.op1->type == op_REGISTER || h->prev->val.two_op.op1->type == op_CONST){
                            h->val.two_op.op1 = h->prev->val.two_op.op1;
                        }
                    }

                }

            }
        }


    }

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

        default:
            break;


    }

    return 0;


}

//Go backwards to find last use of operator
a_asm *get_last_use(a_asm *ins, asm_op *op){

    while (ins != NULL){

        if (ins->ops == 2){
            if (cmp_ops(ins->val.two_op.op1, op)){
                return ins;
            }
            if (cmp_ops(ins->val.two_op.op2, op)){
                return ins;
            }
        }

        ins = ins->prev;
    }
    
    return NULL;
}

//Check if a op is ever used between two instructions
int ever_used(a_asm *from, a_asm *to, asm_op *target){
    while (from != NULL && from != to){
        if (from->ops == 1){
            if (cmp_ops(from->val.one_op.op, target)){
                return 1;
            }
        }
        if (from->ops == 2){
            if (cmp_ops(from->val.two_op.op1, target)){
                return 1;
            }
            if (cmp_ops(from->val.two_op.op2, target)){
                return 1;
            }
        }
        from = from->next;
    }
    return 0;
}