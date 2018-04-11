#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "linked_list.h"
#include "tree.h"
#include "code.h"
#include "memory.h"
#include "symbol.h"



void peephole(a_asm *h){
    a_asm *head;

    a_asm *prev;
    a_asm *next;
    a_asm *current;

    head = h;
    

    //For now we just remove MOV temp, temp instructions, where the two temps are the same
    while (head != NULL){
        printf("Checking instruction: %i\n", head->ins);
        switch (head->ins){
            
            case (MOVQ):
                if (head->val.two_op.op1->type == op_TEMP && head->val.two_op.op2->type == op_TEMP){
                    printf("MOVQ instruction moving t%i to t%i\n", head->val.two_op.op1->val.temp.id, head->val.two_op.op2->val.temp.id);
                    if (head->val.two_op.op1->val.temp.id == head->val.two_op.op2->val.temp.id){
                        printf("Found a MOVQ instruction we want to remove\n");
                        head = head->next;
                        current = head->prev;
                        head->prev->prev->next = head;
                        head->prev = head->prev->prev;
                        free(current);
                        break;
                    }
                } 
                if (head->val.two_op.op1->type == op_LABEL && head->val.two_op.op2->type == op_LABEL){
                    printf("Found stack move ins\n");
                    if (head->val.two_op.op1->stack_offset == head->val.two_op.op2->stack_offset){
                        printf("Found a MOVQ instruction we want to remove\n");
                        head = head->next;
                        current = head->prev;
                        head->prev->prev->next = head;
                        head->prev = head->prev->prev;
                        free(current);
                        break;
                    }
                }
                head = head->next;
                break;

            default:
                head = head->next;
                break;
        }


    }

}