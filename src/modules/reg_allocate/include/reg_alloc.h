#ifndef __reg_alloc_h
#define __reg_alloc_h

#include "code.h"
#include "graph.h"
#include <stdio.h>

#define AVAIL_REGS 14

//Register allocation implementation base on the implementation in the book, page 252

a_asm *reg_alloc(a_asm *h, int local_vars);

//Build interference graph and bit matrix
void *build_interference_graph(a_asm *program);

//Check if a instruction is a MOVQ instruction
int is_move_instruction(a_asm *instruction);

//Checks if register is a machine register
int is_precolored(int i);

//Checks if register is a temporary register
int is_initial(int i);

//Adds a instruction to a registers move list
void add_to_move_list(int i);

//Adds edge in adj_set
void reg_add_edge(int u, int v);

//Checks if a node already exists in a graph. If it does not, creates and returns a new node.
graph_node *check_for_node(int i);

//Adds an adjacency in adj_list between registers. Also increments degree
void add_to_adj_list(int owner, int target);

void show_adj_graph(FILE *out, graph_nodelist *list);

int get_reg(asm_op *op);


#endif