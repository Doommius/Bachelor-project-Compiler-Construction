#ifndef __reg_alloc_h
#define __reg_alloc_h

#include "code.h"
#include "linked_list.h"

typedef struct temporary_meta {
    unsigned importance;
    unsigned temp_id;
    int address;			// Where to put it, if too many registers in use at the same time.
    unsigned *connected;	// length = get_num_temps()-1. Position is name of temporary
} temporary_meta;

typedef struct temporary {
    unsigned temp_id;
	unsigned set; // 1 or 0.
    temporary_meta *meta;
} temporary;


#define AVAIL_REGS 10

void liveness_analysis(a_asm *head);

void forward_analysis(a_asm *head);

void backward_analysis(a_asm *tail);

void check_node_type(a_asm *item);

void add_uses(a_asm *item, int id);

void add_defs(a_asm *item, int id);

void copy_uses(a_asm *item);

void copy_defs(a_asm *item);

void add_defs_remove_uses(a_asm *item, int id);

void copy_content(linked_list *current, linked_list *copy_from);

void make_in_out(a_asm *head, a_asm *node);

void graph_analysis(temporary_meta **meta);

a_asm *reg_alloc(a_asm *h, int local_vars);

void allocate(asm_op **op);


#endif