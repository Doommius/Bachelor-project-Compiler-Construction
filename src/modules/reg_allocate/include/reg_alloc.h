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

void make_in_out(a_asm *head, a_asm *node);

void graph_analysis(temporary_meta **meta);

void set_temp(asm_op *operator, int pos, temporary_meta *temp_meta, temporary *temp);

void set_connected(asm_op *self, temporary_meta **meta);

void init_temp_register_array(temporary_meta *temp_array);

int exists_in_temporary_array(asm_op *operation, temporary *temp_array, unsigned length);

a_asm *reg_alloc(a_asm *h, int local_vars);

void allocate(asm_op **op);


#endif