#ifndef __liveness_analysis_h
#define __liveness_analysis_h

#include <stdio.h>
#include "code.h"
#include "graph.h"

void liveness_analysis(a_asm *program);

void set_op_bit(asm_op *op, BITVECTOR use, BITVECTOR def, int used, int defined);

void set_use_def(a_asm *op);

graph *build_flowgraph(a_asm *program);

graph_nodelist *find_label_node(graph *g, char *label);

void print_sets(graph_nodelist *list);

//Reverse fix point analysis
void analysis(graph *g);

void test_known_program();

#endif
