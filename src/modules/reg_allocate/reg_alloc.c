#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "code.h"
#include "linked_list.h"
#include "reg_alloc.h"
#include "liveness_analysis.h"
#include "graph.h"
#include "stack.h"
#include "memory.h"

//Node work-lists, sets and stacks
BITVECTOR simplify_worklist;            //List of low-degree non-move-related nodes
BITVECTOR freeze_worklist;              //Low-degree move-related nodes
BITVECTOR spill_worklist;               //High-degree nodes
BITVECTOR spilled_nodes;                //Nodes marked for spilling during this round; Initially empty
BITVECTOR coalesced_nodes;              //Registers that have been coalesced
BITVECTOR colored_nodes;                //Nodes succesfully colored
struct stack *select_stack;             //Stack containing temporaries removed from the graph

//Move sets
BITVECTOR coalesced_moves;              //Moves that have been coalesced
BITVECTOR constrained_moves;            //Moves whose source and target interfere
BITVECTOR frozen_moves;                 //Moves that will no longer be considered for coalescing
BITVECTOR worklist_moves;               //Moves enabled for possible coalescing
BITVECTOR active_moves;                 //Moves not yet ready for coalescing

//Other data structures
struct graph *adj_set;                  //The set of interference edges;
BITVECTOR *adj_list;                    //Adjacency list representation of the graph
int *degree;                            //An array containing the current degree of each node;
BITVECTOR *move_list;                   //A mapping from a node to the list of moves it is associated with
int *alias;                             //When a move (tU, tV) has been coalesced, and V put in coalesced_nodes, then alias[V] = U
int *color;                             //The color cosen by the algorithm for a node; for precolored nodes this is initialized to the given color.

int temps;
int line;






a_asm *reg_alloc(a_asm *h, int local_vars){
    printf("Allocating regs\n");
    struct a_asm *head;
    struct a_asm *tail;
    struct a_asm *target1;
    struct a_asm *target2;
    head = NULL;
    tail = NULL;

    liveness_analysis(h);

    temps = get_temps();

    //Initialize data structures                   
    simplify_worklist = init_vector();         
    freeze_worklist = init_vector();              
    spill_worklist = init_vector();              
    spilled_nodes = init_vector();             
    coalesced_nodes = init_vector();               
    colored_nodes = init_vector();               
    select_stack = init_stack();            

    
    coalesced_moves = init_vector();             
    constrained_moves = init_vector(); 
    frozen_moves = init_vector();              
    worklist_moves = init_vector();              
    active_moves = init_vector();                

    
    adj_set = init_graph();             
    adj_list = malloc(sizeof(BITVECTOR) * temps);                    
    degree = malloc(sizeof(int) * temps);                          
    move_list = malloc(sizeof(BITVECTOR) * temps);                 
    alias = malloc(sizeof(int) * temps);                      
    color = malloc(sizeof(int) * temps);

    //Initialize lists
    for (int j = 0; j < temps; j++){
        adj_list[j] = init_vector();
        degree[j] = 0;
        move_list[j] = init_vector();
    }

    //Give registers a color
    int i = 0;
    for (; i < AVAIL_REGS; i++){
        color[i] = i;
    }
    
    //Color the rest of the nodes
    for(; i < temps; i++){
        color[i] = -1;
        
    }

    line = 0;

    build_interference_graph(h);
    printf("Interference graph:\n");

    show_adj_graph(stdout, get_nodes(adj_set));

    return h;


}


//Build interference graph and bit matrix
void *build_interference_graph(a_asm *program){
    BITVECTOR live;
    while (program != NULL){
        live = program->new;
        if (is_move_instruction(program)){
            live = vector_difference(live, program->use);
            //Corresponds to forall n in def(I) U use(I), since MOVQ consists of two ops
            add_to_move_list(get_reg(program->val.two_op.op1));

            add_to_move_list(get_reg(program->val.two_op.op2));
            set_bit(worklist_moves, line);
            
            
        }
        live = vector_union(live, program->def);
        for (int i = 0; i < temps; i++){
            if (get_bit(program->def, i)){

                for (int j = 0; j < temps; j++){
                    if (get_bit(live, j)){
                        reg_add_edge(j, i);

                    }
                }

            }
        }
        live = vector_union(program->use, vector_difference(live, program->def));

        line++;
        program = program->next;
    }


}


//Check if a instruction is a MOVQ instruction
int is_move_instruction(a_asm *instruction){
    //Check that both operators are not constants
    if (instruction->ins == MOVQ && (get_reg(instruction->val.two_op.op1) != -1) && (get_reg(instruction->val.two_op.op2) != -1)){
        return 1;

    }
    return 0;
}

//Checks if register is a machine register
int is_precolored(int i){
    if (i < AVAIL_REGS){
        return 1;
    }
    return 0;
}

//Checks if register is a temporary register
int is_initial(int i){
    if ((i < get_temps()) && (i >= AVAIL_REGS)){
        return 1;
    }
    return 0;
}

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


//Adds a instruction to a registers move list
void add_to_move_list(int i){
    BITVECTOR temp = move_list[i];
    set_bit(temp, line);
}

//Adds edge in adj_set - already a function called "add_edge"
void reg_add_edge(int u, int v){
    struct graph_node *temp1;
    struct graph_node *temp2;
    temp1 = check_for_node(u);
    temp2 = check_for_node(v);

    if (!(check_edge(temp1, temp2)) && (temp1 != temp2)){
        add_edge(temp1, temp2);
        add_edge(temp2, temp1);
        if (!is_precolored(u)){
            add_to_adj_list(u, v);
        }
        if (!is_precolored(v)){
            add_to_adj_list(v, u);
        }
    }

}

//Checks if a node already exists in a graph. If it does not, creates and returns a new node.
graph_node *check_for_node(int i){
    struct graph_nodelist *list;
    struct graph_node *node;
    int temp;
    list = get_nodes(adj_set);
    while (list != NULL){
        temp = (int) get_data(list->head);
        if (temp == i){
            return list->head;
        }
        list = list->next;
    }

    node = insert_node(adj_set, i);
    return node;


}


//Adds an adjacency in adj_list between registers. Also increments degree
void add_to_adj_list(int owner, int target){
    BITVECTOR temp = adj_list[owner];
    set_bit(temp, target);
    degree[owner]++;
}


void show_adj_graph(FILE *out, graph_nodelist *list){
    struct graph_nodelist *temp;
    struct graph_nodelist *succ;
    struct graph_node *node;
    int val1;
    int val2;
    temp = list;
    while (temp != NULL){
        node = temp->head;
        val1 = get_data(temp->head);
        fprintf(out, " (%d): ", val1);
        succ = get_succs(node);
        while (succ != NULL){
            val2 = (int) get_data(succ->head);
            fprintf(out, "%d ", val2);
            succ = succ->next;
        }
        fprintf(out, "\n");
        temp = temp->next;

    }
}