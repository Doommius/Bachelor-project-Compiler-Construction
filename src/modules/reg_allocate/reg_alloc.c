#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "code.h"
#include "linked_list.h"
#include "reg_alloc.h"

void *liveness_analysis(a_asm *tail) {

    a_asm *original_tail = tail;

    temporary_meta *temp_array; // Meta data about temporary

    temporary **temp_matrix; // Usage of temporary at point in code

    // Need to know order of declarations and statements

    temp_array = (temporary_meta *)malloc(sizeof(temporary_meta) * get_num_temps());
    temp_matrix = (temporary *)malloc(sizeof(temporary *) * asm_list_length(tail));

    init_register_array(temp_array);

    asm_op *op1;
    asm_op *op2;

    unsigned y = 0;
    unsigned x = 0;
    unsigned runs = 0;

    while (tail != NULL && runs < 2) {
        temp_matrix[y] = (temporary *)malloc(sizeof(temporary) * get_num_temps());
        if (&tail->val.two_op) {
            op1 = tail->val.two_op.op1;
            op2 = tail->val.two_op.op2;
            int op1_position = exists_in_temporary_array(op1, temp_matrix[y], get_num_temps());
            int op2_position = exists_in_temporary_array(op2, temp_matrix[y], get_num_temps());

            set_temp(op1, op1_position, temp_array, temp_matrix[y]);
            set_temp(op2, op2_position, temp_array, temp_matrix[y]);
        } else if(&tail->val.one_op) {
            op1 = tail->val.one_op.op;

            int op1_position = exists_in_temporary_array(op1, temp_matrix[y], get_num_temps());

            set_temp(op1, op1_position, temp_array, temp_matrix[y]);
        }
    }
}

void set_temp(asm_op *operator, int pos, temporary_meta *temp_meta, temporary *temp) {
    if (pos == -1) {
        for (unsigned i = 0; i < get_num_temps(); ++i) {
            if (&temp[i].temp_id == NULL) {
                temp[i].temp_id = operator->val.temp.id;
                temp[i].meta = &temp_meta[i];
                ++temp[i].meta->importance;
            }
        }
    } else {
        temp[pos].temp_id = operator->val.temp.id;
                temp[pos].meta = &temp_meta[operator->val.temp.id];
                ++temp[pos].meta->importance;
    }
}

void init_register_array(temporary_meta *temp_array) {
    for (int i = 0; i < get_num_temps(); ++i) {
        temp_array[i].address = 0;
        temp_array[i].temp_id = i;
        temp_array[i].importance = 0;
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
        if (temp_array[i].temp_id == operator->val.temp.id) {
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
