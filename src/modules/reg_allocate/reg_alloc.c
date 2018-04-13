#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "code.h"
#include "linked_list.h"
#include "reg_alloc.h"

void liveness_analysis(a_asm *head) {

    forward_analysis(head);

    backward_analysis(head);

    /**
     * @brief Assign register values.
     * 
     */
    //graph_analysis(temp_array);
}

void graph_analysis(temporary_meta **meta) {
    // TODO:
}

/**
 * @brief 
 * 
 * @param head 
 */
void forward_analysis(a_asm *node) {
    a_asm *head = node;
    while (node != NULL) {
        switch (node->ins) {
        case JMP:
            // Add successor to node
            a_asm *item = find_in_flow(head, &node->val.one_op.op->type, node->val.one_op.op);
            node->successors = linked_list_init(item);

            // Add predecessor to successor node
            if (!item->predecessors) {
                item->predecessors = linked_list_init(node);
            } else {
                linked_list_insert_tail(item->predecessors, node);
            }
            break;
        case JNE:
            make_in_out(head, node);
            break;
        case JG:
            make_in_out(head, node);
            break;
        case JL:
            make_in_out(head, node);
            break;
        case JGE:
            make_in_out(head, node);
            break;
        case JLE:
            make_in_out(head, node);
            break;
        case JE:
            make_in_out(head, node);
            break;
        }

        node = node->next;
    }
}

/**
 * @brief Creates in and out sets 
 * 
 * @param head 
 * @param node 
 */
void make_in_out(a_asm *head, a_asm *node) {
    // Add successors node
    a_asm *item_jmp = find_in_flow(head, &node->val.one_op.op->type, node->val.one_op.op);
    node->successors = linked_list_init(item_jmp);

    a_asm *item_nojmp = node->next;
    node->successors = linked_list_insert_tail(node->successors, item_nojmp);

    // Add node to successors' predecessors
    // Add node to item_jmp's predecessor
    if (!item_jmp->predecessors) {
        item_jmp->predecessors = linked_list_init(node);
    } else {
        linked_list_insert_tail(item_jmp->predecessors, node);
    }

    // Add node to item_nojmp's predecessor
    if (!item_nojmp->predecessors) {
        item_nojmp->predecessors = linked_list_init(node);
    } else {
        linked_list_insert_tail(item_nojmp->predecessors, node);
    }
}

/**
 * @brief
 * 
 * @param tail 
 */
void backward_analysis(a_asm *node) {
	a_asm *head = node;


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
