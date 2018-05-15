#include <stdio.h>
#include <string.h>
#include "code.h"
#include "bit_vector.h"
#include "reg_alloc.h"
#include "graph.h"
#include "liveness_analysis.h"
#include "print_asm.h"
#include "main.h"

void set_op_bit(asm_op *op, BITVECTOR use, BITVECTOR def, int used, int defined) {
	int index;

	switch (op->type) {

	case (op_REGISTER):
	case (op_TEMP):
		index = get_reg(op);
		if (index != -1) {
			if (used) {
				set_bit(use, index);
			}
			if (defined) {
				set_bit(def, index);
			}
		}
		break;

	case (op_STACK_LOC):
		set_op_bit(op->val.stack.reg, use, def, 1, 0);
		break;

	case (op_MEM_LOC):
		set_op_bit(op->val.mem_index_reg, use, def, 1, 0);
		break;

	case (op_LEA):
		set_op_bit(op->val.lea.reg, use, def, 1, 0);
		break;

	default:
		break;
	}
}


void set_use_def(a_asm *op){
    
    char comment[256] = "";
    char buffer[128] = "";
    BITVECTOR use;
    BITVECTOR def;
    use = init_vector();
    def = init_vector();
    switch(op->ins){

        case (MOVQ):
            set_op_bit(op->val.two_op.op1, use, def, 1, 0);
            set_op_bit(op->val.two_op.op2, use, def, 0, 1);
            break;


        case (IMUL):
        case (IDIV):
            set_op_bit(reg_RAX, use, def, 1, 1);
            set_op_bit(op->val.one_op.op, use, def, 1,0);
            break;

        case (ADDQ):
        case (XORQ):
        case (SARQ):
        case (LEAQ):
            set_op_bit(op->val.two_op.op1, use, def, 1, 0);
            set_op_bit(op->val.two_op.op2, use, def, 1, 1);
            break;

        case (SUBQ):
            set_op_bit(op->val.two_op.op1, use, def, 1, 1);
            set_op_bit(op->val.two_op.op2, use, def, 1, 0);
            break;
        

        case (CMP):
            set_op_bit(op->val.two_op.op1, use, def, 1, 0);
            set_op_bit(op->val.two_op.op2, use, def, 1, 0);
            break;

        case (PUSH):
            set_op_bit(op->val.one_op.op, use, def, 1, 0);
            break;
        
        case (POP):
            set_op_bit(op->val.one_op.op, use, def, 0, 1);
            break;

        case (CDQ):
            //Manually set the registers used and defined by CDQ, since CDQ has no register arguments
            set_bit(use, 0);
            set_bit(def, 0);
            set_bit(def, 3);
            break;

        case (RET):
            //RET uses RAX when storing the return value
            set_bit(use, 0);
            break;

        case (CALL):
            //CALL defines RAX, since thats where the return value will be placed
            set_bit(def, 0);
            

            //Special case for "printf"
            if (strcmp(op->val.one_op.op->val.label_id, "printf") == 0){
                set_bit(use, 0);
                set_bit(use, 4);
                set_bit(use, 5);

                set_bit(def, 0);
                set_bit(def, 2);
                set_bit(def, 3);
                set_bit(def, 4);
                set_bit(def, 5);
                set_bit(def, 6);
                set_bit(def, 7);
                set_bit(def, 8);
                set_bit(def, 9);
                break;
            }

            //If passing less than PLACE_IN_REGS arguments, define those registers as used
            if (op->func_args <= PLACE_IN_REGS){
                for (int i = 0; i < op->func_args; i++){
                    set_bit(use, AVAIL_REGS-i-1);
                }
            }
            break;

        //Used in test, not actually an instruction we use
        case (ANDQ):
            set_op_bit(op->val.two_op.op1, use, def, 1, 0);
            set_op_bit(op->val.two_op.op2, use, def, 0, 1);
            break;
        
        


        

        default:
            break;

    }

    op->use = use;
    op->def = def;

    // sprintf(comment, "use: %s, def: %s", vector_as_text(use, buffer), vector_as_text(def, buffer));
    // op->comment = strcpy(comment, comment);
    

}

void liveness_analysis(a_asm *program) {
	struct graph *flowgraph;
	struct a_asm *temp;
	temp = program;
	int line;
	line = 1;

	flowgraph = build_flowgraph(program);

	if (verbose) {
		show_graph(stdout, get_nodes(flowgraph));
	}

	analysis(flowgraph);
}

graph *build_flowgraph(a_asm *program) {
	struct a_asm *temp;
	struct graph *g;
	struct graph_node *prev;
	struct graph_nodelist *list;
	struct graph_nodelist *jump_target;
	temp = program;
	g = init_graph();
	prev = NULL;

	//Init - create sets for instruction, and insert it as a node in the graph
	while (temp != NULL) {
		temp->old = init_vector();
		temp->new = init_vector();
		set_use_def(temp);

		insert_node(g, temp);
		temp = temp->next;
	}

	//Add edges between nodes in graph
	list = get_nodes(g);
	while (list != NULL) {
		if (prev != NULL) {
			add_edge(prev, list->head);
		}
		prev = list->head;
		temp = (struct a_asm *)get_data(list->head);

		switch (temp->ins) {

		case (JMP):
			prev = NULL;
			jump_target = find_label_node(g, temp->val.one_op.op->val.label_id);
			add_edge(list->head, jump_target->head);
			break;

		case (JNE):
		case (JE):
		case (JL):
		case (JLE):
		case (JG):
		case (JGE):
			jump_target = find_label_node(g, temp->val.one_op.op->val.label_id);
			add_edge(list->head, jump_target->head);
			if (list->next != NULL) {

				add_edge(list->head, list->next->head);
			}
			break;

		case (RET):
			prev = NULL;
			break;

		default:
			break;
		}
		list = list->next;
	}

	return g;
}

//Find the node corresponding to the given label
graph_nodelist *find_label_node(graph *g, char *label) {
	struct graph_nodelist *list;
	struct a_asm *temp;
	list = get_nodes(g);

	while (list != NULL) {
		temp = (struct a_asm *)get_data(list->head);
		if (temp->ins == LABEL) {
			if (strcmp(temp->val.label.label_id, label) == 0) {
				return list->next;
			}
		}
		list = list->next;
	}

	return NULL;
}

//"Turbo" fix point analysis
void analysis(graph *g) {
	struct graph_nodelist *list;
	struct graph_nodelist *succ;
	struct a_asm *temp;
	struct a_asm *successor;
	int changes;
	int iterations;
	BITVECTOR old;
	BITVECTOR new;
	BITVECTOR tempV;

	while (1) {
		changes = 0;
		iterations++;
		list = g->last;
		while (list != NULL) {

			temp = (struct a_asm *)get_data(list->head);
			old = temp->old;

			temp->new = calc_new_set(list);

			changes = changes | !vector_compare(old, temp->new);

			temp->old = temp->new;
			list = list->prev;
		}

		if (!changes) {
			if (verbose) {
				print_sets(g->nodes);
			}
			break;
		}
	}
}

BITVECTOR calc_new_set(graph_nodelist *list) {
	struct a_asm *temp;
	struct a_asm *successor;
	struct graph_nodelist *succ;
	BITVECTOR tempV;

	temp = (struct a_asm *)get_data(list->head);
	succ = get_succs(list->head);
	tempV = init_vector();
	while (succ != NULL) {

		successor = (struct a_asm *)get_data(succ->head);
		if (succ->head->key > list->head->key) {

			tempV = vector_union(tempV, successor->new);

		} else {
			tempV = vector_union(tempV, successor->old);
		}
		succ = succ->next;
	}
	temp->new = vector_union(temp->use, vector_difference(tempV, temp->def));

	return temp->new;
}

void print_sets(graph_nodelist *list) {
	struct a_asm *temp;
	struct graph_node *node;
	while (list != NULL) {
		node = list->head;
		temp = (struct a_asm *)get_data(node);
		if (temp->ins != LABEL && temp->ins != JMP) {
			printf("Node %d, use: ", node->key);
			vector_print(temp->use);
			printf(" def: ");
			vector_print(temp->def);
			printf(" in: ");
			vector_print(temp->new);
			printf("\n");
		}
		list = list->next;
	}
}