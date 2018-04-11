#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "code.h"
#include "linked_list.h"

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