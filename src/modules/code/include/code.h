#ifndef __code_h
#define __code_h

#include "tree.h"
#include "linked_list.h"
#include "bit_vector.h"


extern int temps;

//Abstract assembly node, with a build in linked list.
//To be expanded
typedef struct a_asm {
	ASM_kind ins;
	struct a_asm *prev;
	struct a_asm *next;
	char *comment;
	int ops;
	int func_args;

	/**
	 * 
	 * Should probably contain information about the head and tail of the current block
	 * 
	 */

	union {

		struct {
			LABEL_kind type;
			int func_vars;
			char *label_id;

		} label;
		struct {
			struct asm_op *op1;
			struct asm_op *op2;
		} two_op;

		struct {
			struct asm_op *op;
		} one_op;
	} val;

	BITVECTOR use;
	BITVECTOR def;
	BITVECTOR old;
	BITVECTOR new;


} a_asm;

typedef struct asm_op {
	OP_kind type;
	int stack_offset;
	union {
		char *reg_id;
		char *label_id;
		char *const_id;
		struct {
			int id;
			struct asm_op *spill;
		} temp;
		struct {
			struct asm_op *reg;
			int offset;
		} stack;
		int intconst;
	} val;
	
} asm_op;



a_asm *generate_program(body *body);

a_asm *generate_body(body *body, char *start_label, char *end_label, head *head);

a_asm *generate_function(function *func);

a_asm *generate_head(head *h);

a_asm *generate_pdl(par_decl_list *pdl, int args);

a_asm *generate_vdl(var_decl_list *vdl, int *offset, int args);

a_asm *generate_dlist(decl_list *dlist);

a_asm *generate_decl(declaration *decl);

a_asm *generate_slist(statement_list *slist);

a_asm *generate_stmt(statement *stmt);

a_asm *generate_var(variable *var);

a_asm *generate_exp(expression *exp);

a_asm *generate_term(term *term);

a_asm *generate_elist(exp_list *elist, int *count);

void asm_insert(a_asm **head, a_asm **tail, a_asm **new);

void asm_insert_one(a_asm **head, a_asm **tail, a_asm **next);

a_asm *find_tail(a_asm *node);

void add_2_ins(a_asm **head, a_asm **tail, ASM_kind ins, asm_op *op1, asm_op *op2, char *comment);

void add_1_ins(a_asm **head, a_asm **tail, ASM_kind ins, asm_op *op1, char *comment);

void add_ins(a_asm **head, a_asm **tail, ASM_kind ins, char *comment);

void add_label(a_asm **head, a_asm **tail, char *label, char *comment);

void add_func_label(a_asm **head, a_asm **tail, char *label, char *comment, int vars);

int local_init(decl_list *dlist);

asm_op *make_op_const(int i);

asm_op *make_op_temp();

asm_op *make_op_label(char *label);

asm_op *make_op_stack_loc(int offset, asm_op **reg);

asm_op *make_op_spill();

void make_cmp_label(char *buffer);

void make_end_cmp_label(char *buffer);

void make_bool_label(char *buffer); 

void make_else_label(char *buffer);

void make_if_label(char *buffer);

void make_loop_start_label(char *buffer);

void make_loop_end_label(char *buffer);

a_asm *get_return_reg(a_asm *tail);

void init_regs();

void get_next(a_asm **head, a_asm **tail);

void add_simple_start(a_asm **head, a_asm **tail);

void add_simple_end(a_asm **head, a_asm **tail);

//Base and stack pointer registers
struct asm_op *reg_RBP;

struct asm_op *reg_RSP;

//General purpose registers we can use. 
struct asm_op *reg_RAX;

struct asm_op *reg_RBX;

struct asm_op *reg_RCX;

struct asm_op *reg_RDX;

struct asm_op *reg_RSI;

struct asm_op *reg_RDI;

struct asm_op *reg_R8;

struct asm_op *reg_R9;

struct asm_op *reg_R10;

struct asm_op *reg_R11;

struct asm_op *reg_R12;

struct asm_op *reg_R13;

struct asm_op *reg_R14;

struct asm_op *reg_R15;

struct asm_op *wrt_INT;

struct asm_op *op_PRINTF;

struct asm_op *wrt_TRUE;

struct asm_op *wrt_FALSE;

struct asm_op *op_STATIC_LINK;




#endif