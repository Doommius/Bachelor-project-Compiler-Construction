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
        a_asm *item;
        switch (node->ins) {
        case JMP:
            // Add successor to node
            item = find_in_flow(head, &node->val.one_op.op->type, node->val.one_op.op);
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
        default:
            // Default case is used to create in and out sets for every other node.
            // Add direct successor to node
            item = node->next;
            node->successors = linked_list_init(item);

            // Add node to predecessor list of the successor
            if (!item->predecessors) {
                item->predecessors = linked_list_init(node);
            }
        }
        node->uses_bool = 0;
        node->defs_bool = 0;
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

    while (node != NULL) {
        a_asm *item;
        if (linked_list_length(node->predecessors) == 1) {
            check_node_type(linked_list_get(node->predecessors, 0));

        } else if (linked_list_length(node->predecessors) == 2) {
            check_node_type(linked_list_get(node->predecessors, 0));
            check_node_type(linked_list_get(node->predecessors, 1));

            // Recurse into second predecessor.
            backward_analysis(linked_list_get(node->predecessors, 1));
        }
        node = node->prev;
    }
}

void check_node_type(a_asm *item) {

    if (linked_list_length(item->successors) == 1) {
        //TODO:  Add uses or defs based on node type
        switch (item->ins) {
        case MOVQ:
            if (item->val.two_op.op1->val.temp.id != NULL) {
                add_uses(item, item->val.two_op.op1->val.temp.id);
            } else if (item->val.two_op.op2->val.temp.id) {
                add_defs(item, item->val.two_op.op2->val.temp.id);
            }
            break;
        case CMP:
            add_uses(item, item->val.two_op.op1->val.temp.id);
            add_uses(item, item->val.two_op.op2->val.temp.id);
            break;
        case ADDQ:
            if (item->val.two_op.op1->val.temp.id != NULL) {
                add_uses(item, item->val.two_op.op1->val.temp.id);
            } else if (item->val.two_op.op2->val.temp.id) {
                add_defs(item, item->val.two_op.op2->val.temp.id);
            }
            break;
        case SUBQ:
            if (item->val.two_op.op1->val.temp.id != NULL) {
                add_uses(item, item->val.two_op.op1->val.temp.id);
            } else if (item->val.two_op.op2->val.temp.id) {
                add_defs(item, item->val.two_op.op2->val.temp.id);
            }
            break;
        case IMUL:
            copy_uses(item);
            copy_defs(item);
        case IDIV:
        case PUSH:
        case POP:
        }

    } else if (linked_list_length(item->successors) == 2) {
        //TODO:  Add uses or defs based on node type
        // Only add uses and defs from previous flow-node.
    }
}

void add_uses(a_asm *item, int id) {
    // TODO: Add switch statements
    if (!linked_list_contains(item->uses, id)) {
        linked_list_insert_tail(item->uses, id);
    }
}

void add_defs(a_asm *item, int id) {
    if (!linked_list_contains(item->defs, id)) {
        linked_list_insert_tail(item->defs, id);
    }
}

void copy_uses(a_asm *item) {
    if (item->uses_bool == 0) {
        if (linked_list_length(item->successors) == 1) {
            for (unsigned i = 0; i < linked_list_length(linked_list_get(item->successors, 0)); ++i) {
                linked_list_insert_tail(item->uses, linked_list_get(linked_list_get(item->successors, 0), i));
            }
        } else if (linked_list_length(item->successors) == 2) {
            for (unsigned i = 0; i < linked_list_length(linked_list_get(item->successors, 0)); ++i) {
                linked_list_insert_tail(item->uses, linked_list_get(linked_list_get(item->successors, 0), i));
            }
            for (unsigned i = 0; i < linked_list_length(linked_list_get(item->successors, 1)); ++i) {
                if (!linked_list_contains(item->uses, linked_list_get(linked_list_get(item->successors, 1), i))) {
                    linked_list_insert_tail(item->uses, linked_list_get(linked_list_get(item->successors, 1), i));
                }
            }
        }
        item->uses_bool = 1;
    } else {
        if (linked_list_length(item->successors) == 1) {
            for (unsigned i = 0; i < linked_list_length(linked_list_get(item->successors, 0)); ++i) {
                if (!linked_list_contains(item->uses, linked_list_get(linked_list_get(item->successors, 0), i))) {
                    linked_list_insert_tail(item->uses, linked_list_get(linked_list_get(item->successors, 0), i));
                }
            }
        } else if (linked_list_length(item->successors) == 2) {
            for (unsigned i = 0; i < linked_list_length(linked_list_get(item->successors, 0)); ++i) {
                if (!linked_list_contains(item->uses, linked_list_get(linked_list_get(item->successors, 0), i))) {
                    linked_list_insert_tail(item->uses, linked_list_get(linked_list_get(item->successors, 0), i));
                }
            }
            for (unsigned i = 0; i < linked_list_length(linked_list_get(item->successors, 1)); ++i) {
                if (!linked_list_contains(item->uses, linked_list_get(linked_list_get(item->successors, 1), i))) {
                    linked_list_insert_tail(item->uses, linked_list_get(linked_list_get(item->successors, 1), i));
                }
            }
        }
        item->uses_bool = 1;
    }
}

void copy_defs(a_asm *item) {
     if (item->defs_bool == 0) {
        if (linked_list_length(item->successors) == 1) {
            for (unsigned i = 0; i < linked_list_length(linked_list_get(item->successors, 0)); ++i) {
                linked_list_insert_tail(item->defs, linked_list_get(linked_list_get(item->successors, 0), i));
            }
        } else if (linked_list_length(item->successors) == 2) {
            for (unsigned i = 0; i < linked_list_length(linked_list_get(item->successors, 0)); ++i) {
                linked_list_insert_tail(item->defs, linked_list_get(linked_list_get(item->successors, 0), i));
            }
            for (unsigned i = 0; i < linked_list_length(linked_list_get(item->successors, 1)); ++i) {
                if (!linked_list_contains(item->defs, linked_list_get(linked_list_get(item->successors, 1), i))) {
                    linked_list_insert_tail(item->defs, linked_list_get(linked_list_get(item->successors, 1), i));
                }
            }
        }
        item->defs_bool = 1;
    } else {
        if (linked_list_length(item->successors) == 1) {
            for (unsigned i = 0; i < linked_list_length(linked_list_get(item->successors, 0)); ++i) {
                if (!linked_list_contains(item->defs, linked_list_get(linked_list_get(item->successors, 0), i))) {
                    linked_list_insert_tail(item->defs, linked_list_get(linked_list_get(item->successors, 0), i));
                }
            }
        } else if (linked_list_length(item->successors) == 2) {
            for (unsigned i = 0; i < linked_list_length(linked_list_get(item->successors, 0)); ++i) {
                if (!linked_list_contains(item->defs, linked_list_get(linked_list_get(item->successors, 0), i))) {
                    linked_list_insert_tail(item->defs, linked_list_get(linked_list_get(item->successors, 0), i));
                }
            }
            for (unsigned i = 0; i < linked_list_length(linked_list_get(item->successors, 1)); ++i) {
                if (!linked_list_contains(item->defs, linked_list_get(linked_list_get(item->successors, 1), i))) {
                    linked_list_insert_tail(item->defs, linked_list_get(linked_list_get(item->successors, 1), i));
                }
            }
        }
        item->defs_bool = 1;
    }
}

void add_defs_remove_uses(a_asm *item, int id) {
    // TODO: Add switch statements
}

void copy_content(linked_list *current, linked_list *copy_from) {
    if (linked_list_length(current) >= 0) {
        do {
            linked_list_insert_tail(current, copy_from);

            copy_from = copy_from->next;
        } while (copy_from != copy_from->meta->head);
    } else {
        do {
            if (!linked_list_contains(current, copy_from->data)) {
                linked_list_insert_tail(current, copy_from->data);
            }
            copy_from = copy_from->next;
        } while (copy_from != copy_from->meta->head);
    }
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
