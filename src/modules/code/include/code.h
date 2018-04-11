#ifndef __code_h
#define __code_h

#include "tree.h"
#include "linked_list.h"


void eval_statement(linked_list *list,statement *s);

void eval_expression(linked_list *list,expression *e);

void code_GEQ_statement(linked_list *list,expression *e);

void code_or_expression(linked_list *list,expression *e);

void code_and_expression(linked_list *list,expression *e);

void code_general_expression(linked_list *list, expression *e, ASM_kind kind);

void code_plus_expression(linked_list *list, expression *e);

linked_list *buildcode(statement_list input);





a_asm *generate_program(body *body);

a_asm *generate_body(body *body, char *start_label, char *end_label);

a_asm *generate_function(function *func);

a_asm *generate_head(head *h);

a_asm *generate_pdl(par_decl_list *pdl);

a_asm *generate_vdl(var_decl_list *vdl, int offset);

a_asm *generate_vtype(var_type *vtype);

a_asm *generate_dlist(decl_list *dlist);

a_asm *generate_decl(declaration *decl);

a_asm *generate_slist(statement_list *slist);

a_asm *generate_stmt(statement *stmt);

a_asm *generate_var(variable *var);

a_asm *generate_exp(expression *exp);

a_asm *generate_term(term *term);

a_asm *generate_alist(act_list *alist);

a_asm *generate_elist(exp_list *elist);

void asm_insert(a_asm **head, a_asm **tail, a_asm **new);

a_asm *find_tail(a_asm *node);

void add_2_ins(a_asm **head, a_asm **tail, ASM_kind ins, asm_op *op1, asm_op *op2, char *comment);

void add_1_ins(a_asm **head, a_asm **tail, ASM_kind ins, asm_op *op1, char *comment);

void add_ins(a_asm **head, a_asm **tail, ASM_kind ins, char *comment);

void add_label(a_asm **head, a_asm **tail, char *label, char *comment);

int local_init(decl_list *dlist);

asm_op *make_op_const(int i);

asm_op *make_op_temp();

asm_op *make_op_label(char *label);

asm_op *make_op_stack_loc(int offset);

void make_cmp_label(char *buffer);

void make_end_cmp_label(char *buffer);

void make_bool_label(char *buffer); 

void make_else_label(char *buffer);

void make_if_label(char *buffer);

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

#endif