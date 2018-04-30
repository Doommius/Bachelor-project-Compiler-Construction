#ifndef __reg_alloc_h
#define __reg_alloc_h

#include "code.h"
#include "graph.h"
#include <stdio.h>

#define AVAIL_REGS 14
#define PLACE_IN_REGS 2

//Register allocation implementation based on the implementation in the book, page 249-256

a_asm *reg_alloc(a_asm *h);

//Build interference graph and bit matrix
void *build_interference_graph(a_asm *program);

//Check if a instruction is a MOVQ instruction
int is_move_instruction(a_asm *instruction);

//Adds a instruction to a registers move list
void add_to_move_list(int i);

//Adds edge in adj_set
void reg_add_edge(int u, int v);

//Checks if a node already exists in a graph. If it does not, creates and returns a new node.
graph_node *check_for_node(int i);

//Builds the initial worklist
void make_worklist();

int move_related(int i);

BITVECTOR node_moves(int i);

void show_adj_graph(FILE *out, graph_nodelist *list);

int get_reg(asm_op *op);

void simplify();

void coalesce();

void freeze();

void select_spill();

BITVECTOR adjacent(int i);

void decrement_degree(int i);

void enable_moves(BITVECTOR bv);

int get_alias(int i);

void add_worklist(int i);

int check_adjacent_low_degree(int i, graph_node *node);

int OK(int i, graph_node *node);

int conservative(int i, int j);

void combine(int u, int v);

void freeze_moves(int i);

int check_invariants();

int degree_invariant();

int simplify_worklist_invariant();

int freeze_worklist_invariant();

int spill_worklist_invariant();

void assign_colors();

a_asm *rewrite_program(a_asm *theprogram);

a_asm *rewrite_spill_reg(asm_op **op, int fetch, asm_op **new_temp);

void replace_temp_op(asm_op **op, asm_op *replacer);

int is_precolored(int reg);
#endif