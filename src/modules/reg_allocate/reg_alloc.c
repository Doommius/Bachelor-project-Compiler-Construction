#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "code.h"
#include "linked_list.h"
#include "reg_alloc.h"

void *liveness_analysis(a_asm *tail) {

    a_asm *original_tail = tail;

    temporary *temp_array;

    a_asm *tail = find_tail(tail);

    // Need to know order of declarations and statements

    temp_array = (temporary *)malloc(sizeof(temporary) * get_num_temps());

    asm_op *op1;
    asm_op *op2;

    unsigned runs = 0;
    while (tail != NULL && runs < 2) {
        if (&tail->val.two_op) {
            op1 = tail->val.two_op.op1;
            op2 = tail->val.two_op.op2;
            int op1_position = exists_in_temporary_array(op1, temp_array, get_num_temps());
            int op2_position = exists_in_temporary_array(op2, temp_array, get_num_temps());

            if(op1_position == -1) {
                
            }
        }
    }
}

/**
 * @brief Searches for element in array. If it finds it, 
 * it will return the position.
 * 
 * @param operator
 * @param temp_array 
 * @param length 
 * @return int 
 */
int exists_in_temporary_array(asm_op *operator, temporary *temp_array, unsigned length) {
    for (unsigned i = 0; i < length; ++i) {
        if (temp_array[i].id == operator->val.temp.id) {
            return i;
        }
    }
    return -1;
}

//Simple register allocation, using the stack
a_asm *reg_alloc(a_asm *h, int local_vars) {
    printf("Allocating regs\n");
    struct a_asm *head;
    struct a_asm *tail;
    struct a_asm *target1;
    struct a_asm *target2;
    head = NULL;
    tail = NULL;

    while (h != NULL) {

        switch (h->ins) {
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

void allocate(asm_op **op) {

    switch ((*op)->type) {
    case (op_TEMP):
        printf("Offset for temp: %d\n", (*op)->val.temp.id);
        (*op) = make_op_stack_loc((*op)->val.temp.id * -8);
        break;

    default:
        break;
    }
}
