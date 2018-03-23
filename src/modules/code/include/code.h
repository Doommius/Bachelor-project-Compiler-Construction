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

a_asm *generate_body(body *body);

a_asm *generate_function(function *func);

a_asm *generate_head(head *head);

a_asm *generate_tail(tail *tail);

a_asm *generate_type(type *type);

a_asm *generate_pdl(par_decl_list *pdl);

a_asm *generate_vdl(var_decl_list *vdl);

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

void asm_insert(a_asm *head, a_asm *tail, a_asm *new);

a_asm *find_tail(a_asm *node);

void add_2_ins(a_asm *head, a_asm *tail, ASM_kind *ins, asm_op *op1, asm_op *op2, char* comment);

void add_1_ins(a_asm *head, a_asm *tail, ASM_kind *ins, asm_op *op1, char* comment);

asm_op *make_op_const(int i);

asm_op *make_op_temp();

void init_regs();

void get_next(a_asm *head, a_asm *tail);

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

#endif