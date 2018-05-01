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
#include "table.h"
#include "rewriter.h"
#include "main.h"

//Register allocation implementation based on the implementation in the book, page 249-256

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

int move_counter;                       //Counts the amount of move instructions in the program
BITVECTOR select_nodes;
table *move_table;                      //Table containing move instructions, used in coalesce

int *uses;                              //Used for spilling, keeps track of all uses/defines for a register
BITVECTOR initial;
BITVECTOR precolored;





a_asm *reg_alloc(a_asm *h){
    
    struct a_asm *head;
    struct a_asm *tail;
    struct a_asm *target1;
    struct a_asm *target2;
    head = NULL;
    tail = NULL;
    int result;



    do {


        liveness_analysis(h);

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
        uses = malloc(sizeof(int) * temps);

        select_nodes = init_vector();
        move_table = table_empty();

        initial = init_vector();
        precolored = init_vector();

        //Initialize lists
        for (int j = 0; j < temps; j++){
            adj_list[j] = init_vector();
            degree[j] = 0;
            uses[j] = 0;
            move_list[j] = init_vector();
        }

        //Give registers a color
        int i = 0;
        for (; i < AVAIL_REGS; i++){
            color[i] = i;
            set_bit(precolored, i);
        }
        
        //Color the rest of the nodes
        for(; i < temps; i++){
            color[i] = -1;
            set_bit(initial, i);
            
        }

        move_counter = 0;

        build_interference_graph(h);
        result = check_invariants();
        if (result != 1){
            printf("Invariants after build did not hold\n");
            return NULL;
        }
        

        make_worklist();


        if (verbose){
            printf("Interference graph:\n");
            show_adj_graph(stdout, get_nodes(adj_set));
        }
        


        int iter;
        iter = 1;
        do {
            if (!vector_empty(simplify_worklist)){
                simplify();
            } else if (!vector_empty(worklist_moves)){
                coalesce();
            } else if (!vector_empty(freeze_worklist)){
                freeze();
                
            } else if (!vector_empty(spill_worklist)){
                select_spill();
            }
            iter++;
        } while ( !(vector_empty(simplify_worklist) && 
                    vector_empty(worklist_moves) && 
                    vector_empty(freeze_worklist) && 
                    vector_empty(spill_worklist)));
        assign_colors();
        if (!vector_empty(spilled_nodes)){
            h = rewrite_program(h);
        }
        
    } while (!vector_empty(spilled_nodes));
    


    return final_rewrite(color, h);


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
            
            //Store the instruction a table, so we can retrieve them later
            table_enter(move_table, (void *) move_counter, program);
            //printf("Inserting move: %d\n", move_counter);
            set_bit(worklist_moves, move_counter);
            

            move_counter++;

            
        }
        live = vector_union(live, program->def);
        for (int i = 0; i < temps; i++){
            if (get_bit(program->def, i)){

                for (int j = 0; j < temps; j++){
                    if (get_bit(live, j)){
                        reg_add_edge(j, i);

                    }
                }

                //Increment use of register for spilling purposes
                uses[i]++;
            } else if (get_bit(program->use, i)){
                uses[i]++;
            }
        }
        live = vector_union(program->use, vector_difference(live, program->def));

        program = program->next;
    }


}

//Builds the initial worklist
void make_worklist(){
    for (int i = 0; i < temps; i++){
        if (get_bit(initial, i)){
            clear_bit(initial, i);
            if (degree[i] >= AVAIL_REGS){
                set_bit(spill_worklist, i);
            } else if (move_related(i)){
                set_bit(freeze_worklist, i);
            } else {
                set_bit(simplify_worklist, i);
            }
        }
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

//Returns a number given an asm_op. This number corresponds to the registers/temps place in a bitvector
int get_reg(asm_op *op){
    
    
    if (op->type == op_TEMP){
        return op->val.temp.id;
    }
    if (op->type == op_STACK_LOC){
        return get_reg(op->val.stack.reg);
    }
    //Would be nice with a switch, but you can't switch on a pointer
    if (op->type == op_REGISTER){

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
    }

    return -1;
}


//Adds a instruction to a registers move list
void add_to_move_list(int i){
    BITVECTOR *temp = move_list[i];
    set_bit(temp, move_counter);
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
        //printf("Adding edge between u: %d, and v: %d\n", u, v);
        if (!get_bit(precolored, u)){
            //printf("u: %d, is not precolored\n", u);
            set_bit(adj_list[u], v);
            degree[u]++;
        }
        if (!get_bit(precolored, v)){
            //printf("v: %d, is not precolored\n", v);
            set_bit(adj_list[v], u);
            degree[v]++;
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

    node = insert_node(adj_set, (void *) i);
    return node;


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

//Returns true if register i is used in any move related instruction
int move_related(int i){
    BITVECTOR temp;
    int test;
    temp = node_moves(i);
    test = !vector_empty(temp);
    return test;

}

//Returns moves instructions where register i is used
BITVECTOR node_moves(int i){
    BITVECTOR temp;
    temp = vector_union(active_moves, worklist_moves);
    temp = vector_intersect(move_list[i], temp);
    return temp;

}

//Push node with low degree onto stack
void simplify(){
    BITVECTOR adj;
    for (int i = 0; i < temps; i++){
        if (get_bit(simplify_worklist, i)){
            clear_bit(simplify_worklist, i);
            set_bit(select_nodes, i);
            stack_push(select_stack, check_for_node(i));
            adj = adjacent(i);
            for (int j = 0; j < temps; j++){
                if (get_bit(adj, j)){
                    decrement_degree(j);

                }
            }
            break;
        }
    }

    
}

//Coalesce nodes
void coalesce(){
    // printf("Moves found: %d\n", move_counter);
    // // printf("Worklist_moves: ");
    // // vector_print(worklist_moves);
    // printf("\n");
    struct a_asm *temp;
    struct graph_node *U;
    struct graph_node *V;
    int x;
    int y;
    int u;
    int v;
    
    for (int i = 0; i < move_counter; i++){
        if (get_bit(worklist_moves, i)){
            //printf("Got move at :%d\n", i);
            temp = (a_asm *) table_look(move_table, (void *) i);
            
            //printf("Casted temp\n");
            x = get_alias(get_reg(temp->val.two_op.op1));
            y = get_alias(get_reg(temp->val.two_op.op2));

            //printf("Got alias\n");

            if (get_bit(precolored, y)){
                u = y;
                v = x;

            } else {
                u = x;
                v = y;
            }
            clear_bit(worklist_moves, i);
            U = check_for_node(u);
            V = check_for_node(v);

            if (u == v){
                set_bit(coalesced_moves, i);
                add_worklist(u);

            } else if (get_bit(precolored, v) || check_edge(U, V)){
                set_bit(constrained_moves, i);
                add_worklist(u);
                add_worklist(v);
            } else if ((get_bit(precolored, u) && (check_adjacent_low_degree(v, U))) ||
                        (!get_bit(precolored, u) && (conservative(u, v)))){
                set_bit(coalesced_moves, i);
                combine(u, v);
                add_worklist(u);

            } else {
                set_bit(active_moves, i);
            }

        }
    }
    
}

void freeze(){
    for (int i = 0; i < temps; i++){
        if (get_bit(freeze_worklist, i)){
            clear_bit(freeze_worklist, i);
            set_bit(simplify_worklist, i);
            freeze_moves(i);
            break;
        }
    }
    
}

//We want to choose a node such that we remove a lot of interferences in the graph. Also, hopefully a node that does not get used too much
void select_spill(){
    int current_best;
    int reg;
    double temp;
    reg = -1;
    for (int i = 0; i < temps; i++){
        if (get_bit(spill_worklist, i)){
            
            temp = uses[i] / (double) degree[i];
            if (reg == -1 || current_best > temp){
                current_best = temp;
                reg = i;
            }
        }
    }
    clear_bit(spill_worklist, reg);
    set_bit(simplify_worklist, reg);
    freeze_moves(reg);



}

//Finds adjacent nodes, and returns then as a new table
BITVECTOR adjacent(int i){
    BITVECTOR temp;
    temp = init_vector();
    temp = vector_difference(adj_list[i], vector_union(select_nodes, coalesced_nodes));
    return temp;
}

void decrement_degree(int i){
    int d;
    BITVECTOR adj;

    degree[i]--;
    d = degree[i];
    if (d == AVAIL_REGS){
        adj = adjacent(i);
        set_bit(adj, i);
        enable_moves(adj);
        clear_bit(spill_worklist, i);
        if (move_related(i)){
            set_bit(freeze_worklist, i);
        } else {
            set_bit(simplify_worklist, i);
        }

    }

}

void enable_moves(BITVECTOR bv){
    BITVECTOR temp;

    for (int i = 0; i < temps; i++){
        if (get_bit(bv, i)){
            temp = node_moves(i);

            for (int j = 0; j < temps; j++){
                if (get_bit(temp, j)){

                    if (get_bit(active_moves, j)){

                        clear_bit(active_moves, j);
                        set_bit(worklist_moves, j);

                    }
                }
            }

        }
    }
}

//Returns alias of register, if coalesced
int get_alias(int i){
    if (get_bit(coalesced_nodes, i)){
        return get_alias(alias[i]);
    } else {
        return i;
    }

}

void add_worklist(int i){
    if ((!get_bit(precolored, i)) && (!move_related(i)) && (degree[i] < AVAIL_REGS) ){

        clear_bit(freeze_worklist, i);
        set_bit(simplify_worklist, i);
        }
}

int check_adjacent_low_degree(int i, graph_node *node){
    BITVECTOR adj;
    adj = adjacent(i);
    for (int j = 0; j < temps; j++){
        if (get_bit(adj, j)){
            if (!OK(j, node)){
                return 0;
            }

        }
    }
    return 1;

}

int OK(int i, graph_node *node){
    struct graph_node *T;
    T = check_for_node(i);
    if ((degree[i] < AVAIL_REGS || get_bit(precolored, i) || check_edge(T, node))){
        return 1;
    } return 0;
}

int conservative(int i, int j){
    int k;
    BITVECTOR adjI;
    BITVECTOR adjJ;
    BITVECTOR temp;
    
    k = 0;
    adjI = adjacent(i);
    adjJ = adjacent(j);
    temp = vector_union(adjI, adjJ);

    for (int n = 0; n < temps; n++){
        if (get_bit(temp, n)){
            if (degree[n] >= AVAIL_REGS){
                k++;
            }
        }
    }   
    return (k < AVAIL_REGS);


}

void combine(int u, int v){
    BITVECTOR adj;
    BITVECTOR temp;
    if (get_bit(freeze_worklist, v)){
        clear_bit(freeze_worklist, v);
    } else {
        clear_bit(spill_worklist, v);
    }

    set_bit(coalesced_nodes, v);
    alias[v] = u;
    
    temp = init_vector();
    //Change is in the errata
    move_list[u] = vector_union(move_list[u], move_list[v]);
    // set_bit(temp, v);
    // enable_moves(temp);
    
    adj = adjacent(v);
    for (int i = 0; i < temps; i++){
        if (get_bit(adj, i)){
            reg_add_edge(i, u);
            decrement_degree(i);
        }
    }

    if (degree[u] >= AVAIL_REGS && get_bit(freeze_worklist, u)){
        clear_bit(freeze_worklist, u);
        set_bit(spill_worklist, u);
    }

}

void freeze_moves(int i){
    struct a_asm *temp;
    int x;
    int y;
    int v;

    for (int j = 0; j < move_counter; j++){
        if (get_bit(node_moves(i), j)){
            temp = (a_asm *) table_look(move_table, (void *) j);
            x = get_alias(get_reg(temp->val.two_op.op1));
            y = get_alias(get_reg(temp->val.two_op.op2));

            if (y == get_alias(i)){
                v = x;
            } else {
                v = y;
            }
            clear_bit(active_moves, j);
            set_bit(frozen_moves, j);

            
            if (vector_empty(node_moves(v)) && degree[v] < AVAIL_REGS){
                clear_bit(freeze_worklist, v);
                set_bit(simplify_worklist, v);
            }

        }
    }
}


void assign_colors(){

    struct graph_node *ins;
    int reg;
    int alias;
    BITVECTOR okColors;
    ins = stack_read(select_stack);
    okColors = init_vector();
    while (ins != NULL){
        stack_pop(select_stack);
        reg = (int) get_data(ins);
        
        if (verbose){
            printf("Colors adjacent to (%d): ", reg);
        }


        for (int i = 0; i < AVAIL_REGS; i++){
            set_bit(okColors, i);
        }

        for (int j = 0; j < temps; j++){
            if (get_bit(adj_list[reg], j)){
                alias = get_alias(j);

                if (verbose){
                    printf(", %d (%d)", j, alias);
                }

                if (get_bit(vector_union(colored_nodes, precolored), alias)){
                    clear_bit(okColors, color[alias]);
                }

            }
        }


        if (vector_empty(okColors)){
            set_bit(spilled_nodes, reg);
            
            if (verbose){
                printf(", Reg is spilled\n");
            }

        } else {
            set_bit(colored_nodes, reg);
            for (int n = 0; n < AVAIL_REGS; n++){
                if (get_bit(okColors, n)){
                    color[reg] = n;

                    if (verbose){
                        printf(", Colored %d\n", n);
                    }

                    break;
                }
            }

        }
        ins = stack_read(select_stack);
        
    }

    for (int i = 0; i < temps; i++){
        if (get_bit(coalesced_nodes, i)){
            color[i] = color[get_alias(i)];

            if (verbose){
                printf("Alias for (%d) = (%d), colored %d\n", i, get_alias(i), color[i]);
            }

        }
    }
}

a_asm *rewrite_program(a_asm *theprogram){
    
    struct a_asm *head;
    struct a_asm *tail;

    struct a_asm *left;
    struct a_asm *right;
    head = NULL;
    tail = NULL;
    while (theprogram != NULL){

        //new_temp is used if we need to create a new temporary and use it later
        struct asm_op *new_temp;
        new_temp = NULL;
        switch (theprogram->ins){
            case (LABEL):
            case (CALL):
            case (JMP):
            case (JE):
            case (JG):
            case (JGE):
            case (JL):
            case (JLE):
            case (JNE):
            case (RET):
            case (CDQ):
            case (IDIV):
            case (IMUL):
            case (BEGIN_CALL):
            case (END_CALL):
                //Nothing to be stored or fetched
                asm_insert_one(&head, &tail, &theprogram);
                break;

            case (MOVQ):
                //Fetch op1 if it's spilled, store op2 if it's spilled
                left = rewrite_spill_reg(&theprogram->val.two_op.op1, 1, NULL);
                asm_insert(&head, &tail, &left);

                asm_insert_one(&head, &tail, &theprogram);

                right = rewrite_spill_reg(&theprogram->val.two_op.op2, 0, NULL);
                asm_insert(&head, &tail, &right);
                break;

            case (ADDQ):
            case (SUBQ):
            case (XORQ):
            case (SARQ):
                //Fetch op1 if it's spilled, fetch op2 into a new temp, 
                left = rewrite_spill_reg(&theprogram->val.two_op.op1, 1, NULL);
                asm_insert(&head, &tail, &left);

                right = rewrite_spill_reg(&theprogram->val.two_op.op2, 1, &new_temp);
                asm_insert(&head, &tail, &right);
                
                asm_insert_one(&head, &tail, &theprogram);

                free(right);

                right = rewrite_spill_reg(&theprogram->val.two_op.op2, 0, &new_temp);
                
                asm_insert(&head, &tail, &right);
                
                break;

            case (PUSH):
                left = rewrite_spill_reg(&theprogram->val.one_op.op, 1, NULL);

                asm_insert(&head, &tail, &left);
                break;

            case (POP):
                asm_insert_one(&head, &tail, &theprogram);
                left = rewrite_spill_reg(&theprogram->val.one_op.op, 0, NULL);

                asm_insert(&head, &tail, &left);
                break;


            default:
                break;
        }

        theprogram = theprogram->next;
        
    }
    return head;

}

//Add a store an fetch before a register, if it's spilled
a_asm *rewrite_spill_reg(asm_op **op, int fetch, asm_op **new_temp){
    struct a_asm *head;
    struct a_asm *tail;
    struct asm_op **target;
    struct asm_op *spill;
    struct asm_op *temp;
    int reg;
    head = NULL;
    tail = NULL;

    reg = get_reg((*op));
    if (reg == -1){
        return NULL;
    }

    if (verbose){
        printf("Spilled nodes: ");
        vector_print(spilled_nodes);
        printf("\n");
    }

    if (get_bit(spilled_nodes, reg)){
        switch ((*op)->type){
            case (op_STACK_LOC):
                target = &(*op)->val.stack.reg;
                break;


            case (op_TEMP):
                target = op;
                break;
        }

        //Create new spill op
        if ((*target)->val.temp.spill == NULL){
            (*target)->val.temp.spill = make_op_spill();
        }
        spill = (*target)->val.temp.spill;

        if (new_temp == NULL){
            temp = make_op_temp();
            replace_temp_op(op, temp);
        } else if ((*new_temp) == NULL){
            temp = make_op_temp();
            (*new_temp) = temp;

        } else {
            temp = (*new_temp);
            replace_temp_op(op, temp);
        }


        if (fetch){
            add_2_ins(&head, &tail, MOVQ, spill, temp, "Fetch spilled temp from stack");

        } else {
            add_2_ins(&head, &tail, MOVQ, temp, spill, "Store spilled temp to stack");
        }
        return head;

    }

}

void replace_temp_op(asm_op **op, asm_op *replacer){
    switch ((*op)->type){

        case (op_TEMP):
            (*op) = replacer;
            break;

        default:
            break;

    }
}


//Check invariants after building graph
int check_invariants(){
    if (!degree_invariant()){
        printf("Degree invariant failed\n");
        return 0;
    }
    if (!simplify_worklist_invariant()){
        printf("Simplify invariant failed\n");
        return 0;
    }
    if (!freeze_worklist_invariant()){
        printf("Freeze invariant failed\n");
        return 0;
    }
    if (!spill_worklist_invariant()){
        printf("Spill invariant failed\n");
        return 0;
    }
    return 1;

}

int degree_invariant(){
    BITVECTOR temp;
    BITVECTOR temp2;
    int length;
    temp = vector_union(simplify_worklist, vector_union(freeze_worklist, spill_worklist));
    length = 0;
    for (int i = 0; i < temps; i++){
        if (get_bit(temp, i)){
            temp2 = vector_intersect(adj_list[i], vector_union(precolored, temp));
            for (int j = 0; j < temps; j++){
                if (get_bit(temp2, j)){
                    length++;
                }
            }
            
            if (!(degree[i] == length)){
                return 0;
            }
            
        }
    }

    return 1;

}

int simplify_worklist_invariant(){
    for (int i = 0; i < temps; i++){
        if (get_bit(simplify_worklist, i)){
            if (!(degree[i] < AVAIL_REGS)){
                return 0;
            }
            
            if (!(vector_empty(vector_intersect(move_list[i], vector_union(active_moves, worklist_moves))))){
                return 0;
            }
        }
    }
    return 1;

}

int freeze_worklist_invariant(){
    for (int i = 0; i < temps; i++){
        if (get_bit(freeze_worklist, i)){
            if (!(degree[i] < AVAIL_REGS)){
                return 0;
            }
            if ((vector_empty(vector_intersect(move_list[i], vector_union(active_moves, worklist_moves))))){
                return 0;
            }
        }
    }
    return 1;

}

int spill_worklist_invariant(){
    for (int i = 0; i < temps; i++){
        if (get_bit(spill_worklist, i)){
            if (!(degree[i] >= AVAIL_REGS)){
                return 0;
            }
        }
    }
    return 1;

}


int is_precolored(int reg){
    if (get_bit(precolored, reg)){
        return 1;
    }
    return 0;
}