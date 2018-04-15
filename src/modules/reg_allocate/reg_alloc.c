#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "code.h"
#include "linked_list.h"
#include "reg_alloc.h"

//Returns a number given an asm_op. This number corresponds to the registers/temps place in a bitvector
int get_reg(asm_op *op){
    switch (op->type){
        
        case (op_TEMP):
            return op->val.temp.id;
            break;
        
        //Would be nice with a switch, but you can't switch on a pointer
        case (op_REGISTER):
            if (op == reg_RAX){
                return 0;
            }
            if (op == reg_RBX){
                return 1;
            }
            if (op == reg_RCX){
                return 2;
            }
            if (op == reg_RDX){
                return 3;
            }
            if (op == reg_RSI){
                return 4;
            }
            if (op == reg_RDI){
                return 5;
            }
            if (op == reg_R8){
                return 6;
            }
            if (op == reg_R9){
                return 7;
            }
            if (op == reg_R10){
                return 8;
            }
            if (op == reg_R11){
                return 9;
            }
            if (op == reg_R12){
                return 10;
            }
            if (op == reg_R13){
                return 11;
            }
            if (op == reg_R14){
                return 12;
            }
            if (op == reg_R15){
                return 13;
            }
            break;

        default:
            break;


    }
    return -1;
}


//Simple register allocation, using the stack
a_asm *reg_alloc(a_asm *h, int local_vars){
    printf("Allocating regs\n");
    struct a_asm *head;
    struct a_asm *tail;
    struct a_asm *target1;
    struct a_asm *target2;
    head = NULL;
    tail = NULL;

    while (h != NULL){

        switch (h->ins){
            case (MOVQ):
            case (CMP):
            case (ADDQ):
            case (SUBQ):
                printf("Allocating first op in 2 op\n");
                allocate(&(h->val.two_op.op1));

                printf("Allocating second op in 2 op\n");
                allocate(&(h->val.two_op.op2));
                

                break;

            case (IMUL):
            case (IDIV):
            case (PUSH):
            case (POP):
                printf("Allocating first op in 1 op\n");
                allocate(&(h->val.one_op.op));
                

                
                break;


        }
        h = h->next;


    }

    return h;


}

void allocate(asm_op **op){

    switch ((*op)->type){
        case (op_TEMP):
            printf("Offset for temp: %d\n", (*op)->val.temp.id);
            (*op) = make_op_stack_loc((*op)->val.temp.id * -8);
            break;
            


        default:
            break;
    }

}