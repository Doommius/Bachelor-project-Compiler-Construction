/**
 * @brief Assembly code generator for the x86 architecture.
 * 
 * @file code.c
 * @author Morten Jæger, Mark Jervelund & Troels Blicher Petersen
 * @date 2018-03-09
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "linked_list.h"
#include "tree.h"
#include "code.h"
#include "memory.h"
#include "symbol.h"
#include "reg_alloc.h"
#include "rewriter.h"
#include "main.h"

extern int temps = AVAIL_REGS; 					//The predefined registers will be the first values in the bitvectors
extern int memSize = 80000;

int cmps = 0;
int ifs = 0;
int loops = 0;
int nullInserts = 0;
int nonNullInserts = 0; 

/**
 * Initialize register operators
 * 
 * Also initialize "wrt_INT" and "op_PRINTF", for printing ints
 * 
 */
void init_regs(){
	reg_RBP = NEW(asm_op);
	reg_RBP->type = op_REGISTER;
	reg_RBP->val.reg_id = "rbp";
	
	reg_RSP = NEW(asm_op);
	reg_RSP->type = op_REGISTER;
	reg_RSP->val.reg_id = "rsp";

	reg_RAX = NEW(asm_op);
	reg_RAX->type = op_REGISTER;
	reg_RAX->val.reg_id = "rax";

	reg_RBX = NEW(asm_op);
	reg_RBX->type = op_REGISTER;
	reg_RBX->val.reg_id = "rbx";

	reg_RCX = NEW(asm_op);
	reg_RCX->type = op_REGISTER;
	reg_RCX->val.reg_id = "rcx";

	reg_RDX = NEW(asm_op);
	reg_RDX->type = op_REGISTER;
	reg_RDX->val.reg_id = "rdx";

	reg_RSI = NEW(asm_op);
	reg_RSI->type = op_REGISTER;
	reg_RSI->val.reg_id = "rsi";

	reg_RDI = NEW(asm_op);
	reg_RDI->type = op_REGISTER;
	reg_RDI->val.reg_id = "rdi";

	reg_R8 = NEW(asm_op);
	reg_R8->type = op_REGISTER;
	reg_R8->val.reg_id = "r8";

	reg_R9 = NEW(asm_op);
	reg_R9->type = op_REGISTER;
	reg_R9->val.reg_id = "r9";

	reg_R10 = NEW(asm_op);
	reg_R10->type = op_REGISTER;
	reg_R10->val.reg_id = "r10";

	reg_R11 = NEW(asm_op);
	reg_R11->type = op_REGISTER;
	reg_R11->val.reg_id = "r11";

	reg_R12 = NEW(asm_op);
	reg_R12->type = op_REGISTER;
	reg_R12->val.reg_id = "r12";

	reg_R13 = NEW(asm_op);
	reg_R13->type = op_REGISTER;
	reg_R13->val.reg_id = "r13";

	reg_R14 = NEW(asm_op);
	reg_R14->type = op_REGISTER;
	reg_R14->val.reg_id = "r14";

	reg_R15= NEW(asm_op);
	reg_R15->type = op_REGISTER;
	reg_R15->val.reg_id = "r15";

	wrt_INT = NEW(asm_op);
	wrt_INT->type = op_CONST;
	wrt_INT->val.const_id = ".wrt_INT";

	wrt_TRUE = NEW(asm_op);
	wrt_TRUE->type = op_CONST;
	wrt_TRUE->val.const_id = ".wrt_TRUE";
	
	wrt_FALSE = NEW(asm_op);
	wrt_FALSE->type = op_CONST;
	wrt_FALSE->val.const_id = ".wrt_FALSE";

	op_PRINTF = NEW(asm_op);
	op_PRINTF->type = op_LABEL;
	op_PRINTF->val.label_id = "printf";

	op_STATIC_LINK = NEW(asm_op);
	op_STATIC_LINK->type = op_LABEL;
	op_STATIC_LINK->val.label_id = "16(%rbp)";

	op_MEM = NEW(asm_op);
	op_MEM->type = op_LABEL;
	op_MEM->val.label_id = "MEM";

	

}

void init_flags() {
	div_zero_flag = 0;
	array_index_flag = 0;
	out_of_mem_flag = 0;
	positive_allocate_flag = 0;
	uninit_var_flag = 0;
}

a_asm *generate_program(body *body){
	struct a_asm *b;
	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;
    

	init_regs();

	if(runtime_checks) {
		init_flags();
	}

	b = generate_body(body, "main", "main_end", NULL);

	asm_insert(&head, &tail, &b);
	add_2_ins(&head, &tail, MOVQ, make_op_const(0), reg_RAX, "Return \"no error\" exit code");
	add_ins(&head, &tail, RET, "Program return");

	add_runtime_checks(&head, &tail);


	return head;
   
}

//Body needs to also get "head", otherwise, fetching the function parameters gets inserted before the function label
a_asm *generate_body(body *body, char *start_label, char *end_label, head *h){
	int vars;

	struct a_asm *sl;
	struct a_asm *dl;
	struct a_asm *head;
    struct a_asm *tail;
	struct a_asm *head2;
	struct a_asm *tail2;
	struct a_asm *func_head;
	struct a_asm *zero_init;
	head = NULL;
	tail = NULL;
	head2 = NULL;
	tail2 = NULL;
	
	vars = local_init(body->d_list);


	dl = generate_dlist(body->d_list);
	asm_insert(&head, &tail, &dl);

	//We want "main" to be the last label in the file.
	add_func_label(&head, &tail, start_label, "Start of body", vars);

	if (h != NULL){
		func_head = generate_head(h);
		asm_insert(&head, &tail, &func_head);
	} else {
		//Init MEM in main
		add_2_ins(&head, &tail, MOVQ, make_op_const(1), op_MEM, "Init MEM");
	}

	zero_init = add_zero_init(body->d_list);
	asm_insert(&head, &tail, &zero_init);

	
	sl = generate_slist(body->s_list);
	asm_insert(&head2, &tail2, &sl);
	

	asm_insert(&head, &tail, &head2);



	add_label(&head, &tail, end_label, "End of body");
	tail->val.label.type = label_FUNC_END;

	return head;
	
}

a_asm *generate_function(function *func){
	struct a_asm *head;
    struct a_asm *tail;
	struct a_asm *b;
	struct a_asm *h;
	head = NULL;
	tail = NULL;


	b = generate_body(func->body, func->start_label, func->end_label, func->head);
	asm_insert(&head, &tail, &b);

	add_ins(&head, &tail, RET, "Return from function");


	return head;
	
}

a_asm *generate_head(head *h){
	struct a_asm *head;
    struct a_asm *tail;
	struct a_asm *pdl;

	head = NULL;
	tail = NULL;

	pdl = generate_pdl(h->list, h->args);
	asm_insert(&head, &tail, &pdl);

	return head;
}

a_asm *generate_pdl(par_decl_list *pdl, int args){
	struct a_asm *head;
    struct a_asm *tail;
	struct a_asm *vdl;
	int offset;
	head = NULL;
	tail = NULL;
    
	if (pdl->kind != pdl_EMPTY){
		offset = 1;
		vdl = generate_vdl(pdl->list, &offset, args);
		asm_insert(&head, &tail, &vdl);
	}

	return head;
}

a_asm *generate_vdl(var_decl_list *vdl, int *offset, int args){
	struct a_asm *head;
    struct a_asm *tail;
	struct a_asm *l;
	struct asm_op *target;
	SYMBOL *s;
	head = NULL;
	tail = NULL;

	s = get_symbol(vdl->vartype->table, vdl->vartype->id);

	//If "PLACE_IN_REGS" or fewer parameters, we want to place it in regs, instead of stack
	if (args <= PLACE_IN_REGS){
		//Create a new temp for the corresponding symbol
		//printf("Args: %d, upper bound: %d\n", args, PLACE_IN_REGS);
		//Use "least used" reg to place parameter in
		target = get_corresponding_reg(AVAIL_REGS - (*offset));
		s->offset = 0;
		s->is_on_stack = 0;
		s->op = make_op_temp();
		add_2_ins(&head, &tail, MOVQ, target, s->op, "Move parameter to register in function");

	} else {
		//Have to add 2, since one place is for static link, and one is for pushing of base pointer in new function
		s->is_on_stack = 1;
		s->offset = ((*offset)+2) * -1;
	}
	
	(*offset)++;
	
	//printf("Offset of var: %s = %d\n", s->name, s->offset);
	if ( vdl->kind == vdl_LIST && vdl->list != NULL){
		l = generate_vdl(vdl->list, offset, args);
		asm_insert(&head, &tail, &l);
	}
    
	return head;
}

a_asm *generate_dlist(decl_list *dlist){
	struct a_asm *head;
    struct a_asm *tail;

	struct a_asm *decl;
	struct a_asm *dl;

	head = NULL;
	tail = NULL;

	if (dlist->kind != dl_EMPTY){
		decl = generate_decl(dlist->decl);
		asm_insert(&head, &tail, &decl);

		dl = generate_dlist(dlist->list);
		asm_insert(&head, &tail, &dl);
		
	}

	return head;
}

a_asm *generate_decl(declaration *decl){
	struct a_asm *head;
    struct a_asm *tail;
	struct a_asm *func;
	head = NULL;
	tail = NULL;

	if (decl->kind == decl_FUNC){
		func = generate_function(decl->val.function);
		asm_insert(&head, &tail, &func);
	}
    

	return head;
}

a_asm *generate_slist(statement_list *slist){
	struct a_asm *stmt;
	struct a_asm *sl;

	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;




	if (slist != NULL){
			
		stmt = generate_stmt(slist->statement);

		asm_insert(&head, &tail, &stmt);

		if (slist->list != NULL){

			sl = generate_slist(slist->list);
			asm_insert(&head, &tail, &sl);
		}
		
	}

	// Could possibly be a struct = {head, tail}, so we didn't have to loop through and find the tail later.
	return head;
    
}

a_asm *generate_stmt(statement *stmt){
	struct a_asm *head;
    struct a_asm *tail;
	char label_else[20];
	char label_ifend[20];
	char label_loop_start[20];
	char label_loop_end[20];
	char label_wrt_true[20];
	char label_wrt_end[20];
	char label_allocate_length[20];
	char label_out_of_mem[20];

	head = NULL;
	tail = NULL;
	struct a_asm *expr;
	struct a_asm *s1;
	struct a_asm *s2;
	struct a_asm *v;
	struct a_asm *mem_op;
	struct a_asm *iter;

	struct asm_op *wrt;
	struct asm_op *ret;
	struct asm_op *length;
	struct asm_op *ifexp;
	struct asm_op *var;
	struct asm_op *assignexp;
	struct asm_op *temp;

	int size;


	switch (stmt->kind){

		//Records - Implementation of record is the excact same as lists, except we don't keep the size of the record at index 0
		case (statement_ALLOCATE):
			size = table_size(stmt->val.allocate.variable->stype->val.record_type->table);
			ret = make_op_temp();
			length = make_op_temp();

			add_2_ins(&head, &tail, MOVQ, make_op_const(size), length, "Move size of record to register");


			mem_op = make_op_temp();
			add_2_ins(&head, &tail, MOVQ, op_MEM, mem_op, "Move pointer to MEM to another register");

			add_2_ins(&head, &tail, MOVQ, length, ret, "Moving length of array to new reg");

			add_2_ins(&head, &tail, ADDQ, make_op_const(1), ret, "Add 1 to array length");
			
			add_2_ins(&head, &tail, MOVQ, ret, make_op_mem_loc(&mem_op), "Move allocated length to mem location");

			v = generate_var(stmt->val.allocate.variable);
			asm_insert(&head, &tail, &v);
			var = get_return_reg(tail);

			add_2_ins(&head, &tail, MOVQ, mem_op, var, "Move mem location to variable");

			add_2_ins(&head, &tail, ADDQ, mem_op, ret, "Add size of record to mem pointer");

			//Insert RTC failure label to check if length if larger than memSize
			if (runtime_checks){
				out_of_mem_flag = 1;
				make_out_of_mem_label(label_out_of_mem);
				add_2_ins(&head, &tail, CMP, make_op_const(memSize), length, "Check if out of memory");
				add_1_ins(&head, &tail, JGE, make_op_label(label_out_of_mem), "Jump to error label");
			}

			add_2_ins(&head, &tail, MOVQ, ret, op_MEM, "Update mem pointer");
			
			//Put size of array into index 0
			temp = make_op_temp();
			add_2_ins(&head, &tail, MOVQ, make_op_const(0), temp, "Moving index 0 to register");

			add_2_ins(&head, &tail, ADDQ, var, temp, "Adding index to start of array");

			add_2_ins(&head, &tail, MOVQ, length, make_op_mem_loc(&temp), "Assigning length of array to index 0");

			add_2_ins(&head, &tail, MOVQ, var, var, "Used to get target for next instruction");

			break;
			
		//Arrays
		case (statement_ALLOCATE_LENGTH):
			expr = generate_exp(stmt->val.allocate.length);
			asm_insert(&head, &tail, &expr);
			length = get_return_reg(tail);
			ret = make_op_temp();
			


			//Insert RTC failure label to check if length <= 0

			if (runtime_checks){
				positive_allocate_flag = 1;
				make_positive_allocate_label(label_allocate_length);
				add_2_ins(&head, &tail, CMP, make_op_const(0), length, "Check if allocated length is positive");
				add_1_ins(&head, &tail, JLE, make_op_label(label_allocate_length), "Jump to error label");
			}

			mem_op = make_op_temp();
			add_2_ins(&head, &tail, MOVQ, op_MEM, mem_op, "Move pointer to MEM to another register");
			
			add_2_ins(&head, &tail, MOVQ, length, ret, "Moving length of array to new reg");

			add_2_ins(&head, &tail, ADDQ, make_op_const(1), ret, "Add 1 to array length");
			
			add_2_ins(&head, &tail, MOVQ, ret, make_op_mem_loc(&mem_op), "Move allocated length to mem location");

			v = generate_var(stmt->val.allocate.variable);
			asm_insert(&head, &tail, &v);
			var = get_return_reg(tail);
			
			add_2_ins(&head, &tail, MOVQ, mem_op, var, "Move mem location to variable");
			
			add_2_ins(&head, &tail, ADDQ, mem_op, ret, "Add length of array to mem register");

			//Insert RTC failure label to check if length if larger than memSize

			if (runtime_checks){
				out_of_mem_flag = 1;
				make_out_of_mem_label(label_out_of_mem);
				add_2_ins(&head, &tail, CMP, make_op_const(memSize), length, "Check if out of memory");
				add_1_ins(&head, &tail, JGE, make_op_label(label_out_of_mem), "Jump to error label");
			}

			add_2_ins(&head, &tail, MOVQ, ret, op_MEM, "Update mem pointer");

			//Put size of array into index 0
			temp = make_op_temp();
			add_2_ins(&head, &tail, MOVQ, make_op_const(0), temp, "Moving index 0 to register");

			add_2_ins(&head, &tail, ADDQ, var, temp, "Adding index to start of array");

			add_2_ins(&head, &tail, MOVQ, length, make_op_mem_loc(&temp), "Assigning length of array to index 0");

			add_2_ins(&head, &tail, MOVQ, var, var, "Used to get target for next instruction");

			break;

		case (statement_RETURN):
			expr = generate_exp(stmt->val.ret);
			asm_insert(&head, &tail, &expr);
			ret = get_return_reg(tail);
			add_2_ins(&head, &tail, MOVQ, ret, reg_RAX, "Return value placed in RAX");
			add_1_ins(&head, &tail, JMP, make_op_label(stmt->function->end_label), "Jump to functions end label");
			break;

		case (statement_WRITE):

			expr = generate_exp(stmt->val.wrt);
			asm_insert(&head, &tail, &expr);
			wrt = get_return_reg(tail);
			if (stmt->val.wrt->stype->type == type_INT){
				//Should probably be some more pushing before creating a print

				//add_1_ins(&head, &tail, PUSH, reg_RAX, "Saving value of RAX before printf call");

				add_ins(&head, &tail, BEGIN_CALL, "Beginning of function call");

				add_2_ins(&head, &tail, MOVQ, wrt_INT, reg_RDI, "First argument for printf");
				add_2_ins(&head, &tail, MOVQ, wrt, reg_RSI, "Second argument for printf");
				add_2_ins(&head, &tail, MOVQ, make_op_const(0), reg_RAX, "No vector arguments");
				add_1_ins(&head, &tail, CALL, op_PRINTF, "Calling printf");

				//add_1_ins(&head, &tail, POP, reg_RAX, "Restoring RAX");
				add_ins(&head, &tail, END_CALL, "End of function call");

			}
			if (stmt->val.wrt->stype->type == type_BOOl){
				make_cmp_label(label_wrt_true);
				make_end_cmp_label(label_wrt_end);

				add_2_ins(&head, &tail, CMP, make_op_const(0), wrt, "Compare boolean operator");
				add_1_ins(&head, &tail, JNE, make_op_label(label_wrt_true), "If true, jump to true part");

				//Should probably be some more pushing before creating a print

				//add_1_ins(&head, &tail, PUSH, reg_RAX, "Saving value of RAX before printf call");

				add_ins(&head, &tail, BEGIN_CALL, "Beginning of function call");

				add_2_ins(&head, &tail, MOVQ, wrt_FALSE, reg_RDI, "First argument for printf");
				add_2_ins(&head, &tail, MOVQ, make_op_const(0), reg_RAX, "No vector arguments");
				add_1_ins(&head, &tail, CALL, op_PRINTF, "Calling printf");

				//add_1_ins(&head, &tail, POP, reg_RAX, "Restoring RAX");
				add_ins(&head, &tail, END_CALL, "End of function call");

				add_1_ins(&head, &tail, JMP, make_op_label(label_wrt_end), "Jump to end of write sequence");
				add_label(&head, &tail, label_wrt_true, "If expression is true, print TRUE");

				add_ins(&head, &tail, BEGIN_CALL, "Beginning of function call");

				add_2_ins(&head, &tail, MOVQ, wrt_TRUE, reg_RDI, "First argument for printf");
				add_2_ins(&head, &tail, MOVQ, make_op_const(0), reg_RAX, "No vector arguments");
				add_1_ins(&head, &tail, CALL, op_PRINTF, "Calling printf");

				//add_1_ins(&head, &tail, POP, reg_RAX, "Restoring RAX");
				add_ins(&head, &tail, END_CALL, "End of function call");

				add_label(&head, &tail, label_wrt_end, "End of write sequence");
			}

			
			break;

		case (statement_IF):
			expr = generate_exp(stmt->val.ifthen.expression);
			asm_insert(&head, &tail, &expr);
			ifexp = get_return_reg(tail);

			ifs++;
			make_if_label(label_ifend);
			
			add_2_ins(&head, &tail, CMP, make_op_const(1), ifexp, "Check if IF expression is true");
			add_1_ins(&head, &tail, JNE, make_op_label(label_ifend), "Expression is false, skip IF part");

			s1 = generate_stmt(stmt->val.ifthen.statement1);
			asm_insert(&head, &tail, &s1);
			add_label(&head, &tail, label_ifend, "End of IF");
			break;

		case (statement_IF_ELSE):
			expr = generate_exp(stmt->val.ifthen.expression);
			asm_insert(&head, &tail, &expr);
			ifexp = get_return_reg(tail);

			make_else_label(label_else);
			make_if_label(label_ifend);
			add_2_ins(&head, &tail, CMP, make_op_const(1), ifexp, "Check if IF expression is true");
			add_1_ins(&head, &tail, JNE, make_op_label(label_else), "Expression is false, jump to ELSE part");

			s1 = generate_stmt(stmt->val.ifthen.statement1);
			asm_insert(&head, &tail, &s1);
			add_1_ins(&head, &tail, JMP, make_op_label(label_ifend), "Skip ELSE part");

			add_label(&head, &tail, label_else, "Start of ELSE");
			s2 = generate_stmt(stmt->val.ifthen.statement2);
			asm_insert(&head, &tail, &s2);

			add_label(&head, &tail, label_ifend, "End of IF");
			break;

		case (statement_ASSIGNMENT):
			expr = generate_exp(stmt->val.assignment.expression);
			asm_insert(&head, &tail, &expr);
			assignexp = get_return_reg(tail);

			v = generate_var(stmt->val.assignment.variable);
			asm_insert(&head, &tail, &v);
			var = get_return_reg(tail);


			add_2_ins(&head, &tail, MOVQ, assignexp, var, "Assigning value to var");
			break;

		case (statement_WHILE):
			
			make_loop_start_label(label_loop_start);
			make_loop_end_label(label_loop_end);

			add_label(&head, &tail, label_loop_start, "Start of while");
			expr = generate_exp(stmt->val.w_loop.expression);
			asm_insert(&head, &tail, &expr);
			ifexp = get_return_reg(tail);

			add_2_ins(&head, &tail, CMP, make_op_const(1), ifexp, "Check if condition in while is true");
			add_1_ins(&head, &tail, JNE, make_op_label(label_loop_end), "If condition is false, jump to end");
			s1 = generate_stmt(stmt->val.w_loop.statement);
			asm_insert(&head, &tail, &s1);
			add_1_ins(&head, &tail, JMP, make_op_label(label_loop_start), "Jump to start of while");
			add_label(&head, &tail, label_loop_end, "End of while");
			break;

		case (statement_FOR):

			make_loop_start_label(label_loop_start);
			make_loop_end_label(label_loop_end);
			
			s1 = generate_stmt(stmt->val.f_loop.assign);
			asm_insert(&head, &tail, &s1);

			add_label(&head, &tail, label_loop_start, "Start of for");
			expr = generate_exp(stmt->val.f_loop.cond);
			asm_insert(&head, &tail, &expr);
			ifexp = get_return_reg(tail);

			add_2_ins(&head, &tail, CMP, make_op_const(1), ifexp, "Check if condition in while is true");
			add_1_ins(&head, &tail, JNE, make_op_label(label_loop_end), "If condition is false, jump to end");

			s2 = generate_stmt(stmt->val.f_loop.body);
			asm_insert(&head, &tail, &s2);

			iter = generate_stmt(stmt->val.f_loop.iter);
			asm_insert(&head, &tail, &iter);


			add_1_ins(&head, &tail, JMP, make_op_label(label_loop_start), "Jump to start of while");
			add_label(&head, &tail, label_loop_end, "End of for");
			break;



		case (statement_LIST):
			s1 = generate_slist(stmt->val.list);
			asm_insert(&head, &tail, &s1);
			break;

		default:
			break;

	}
    
	return head;
}


a_asm *generate_var(variable *var){
	struct a_asm *head;
    struct a_asm *tail;

	struct a_asm *expr;
	struct a_asm *variable;

	struct asm_op *v;
	struct asm_op *temp;
	struct asm_op *static_link;
	struct asm_op *ret;
	struct asm_op *mem;
	struct asm_op *array_length;

	char label_uninit[20];
	char label_index[20];

	int depth;
	head = NULL;
	tail = NULL;


	SYMBOL *s;

	switch (var->kind){

		case (var_ID):
			s = get_symbol(var->table, var->id);
			depth = get_symbol_depth(var->table, var->id);
			//printf("Depth of symbol %s: %d\n", var->id, depth);
			
			//If depth == 0, local variable, no need to check the static link
			if (depth == 0){
				if (!s->is_on_stack){
					v = s->op;
					add_2_ins(&head, &tail, MOVQ, v, v, "Var is in this register");
				} else {
					v = make_op_stack_loc(s->offset, &reg_RBP);
					add_2_ins(&head, &tail, MOVQ, v, v, "Var is on stack");

				}	
			} else {
				
				static_link = make_op_temp();
				add_2_ins(&head, &tail, MOVQ, op_STATIC_LINK, static_link, "Copy static link to new reg");

				//Resolve static link
				depth--;

				while (depth > 0){
					temp = make_op_stack_loc(-2, &static_link);
					static_link = make_op_temp();
					add_2_ins(&head, &tail, MOVQ, temp, static_link, "Copying static link");
					depth--;

				}
				v = make_op_stack_loc((s->offset), &static_link);

				add_2_ins(&head, &tail, MOVQ, v, v, "Used to get target for next instruction");


			}

			
			break;

		case (var_EXP):
			expr = generate_exp(var->val.exp.exp);
			asm_insert(&head, &tail, &expr);
			ret = get_return_reg(tail);

			variable = generate_var(var->val.exp.var);
			asm_insert(&head, &tail, &variable);
			v = get_return_reg(tail);
			
			//Insert RTC failure label to check if array is initialized, if index is larger than the array's size, and if index is smaller than 0
			
			if (runtime_checks){
				uninit_var_flag = 1;
				array_index_flag = 1;
				make_array_index_label(label_index);
				make_uninitialized_var_label(label_uninit);

				add_2_ins(&head, &tail, CMP, make_op_const(0), v, "Check if variable is initialized");
				add_1_ins(&head, &tail, JE, make_op_label(label_uninit), "Jump to error label");
				
				temp = make_op_temp();
				add_2_ins(&head, &tail, MOVQ, make_op_const(0), temp, "Want to get value at index 0 of array");

				add_2_ins(&head, &tail, ADDQ, v, temp, "Get value at index 0");
			
				mem = make_op_mem_loc(&temp);
				array_length = make_op_temp();
				add_2_ins(&head, &tail, MOVQ, mem, array_length, "Copy length of array to reg");

				add_2_ins(&head, &tail, CMP, array_length, ret, "Compare index to size");
				add_1_ins(&head, &tail, JGE, make_op_label(label_index), "Jump to error label");

				add_2_ins(&head, &tail, CMP, make_op_const(0), ret, "Compare index to 0");
				add_1_ins(&head, &tail, JL, make_op_label(label_index), "Jump to error label");

			}

			temp = make_op_temp();
		
			add_2_ins(&head, &tail, LEAQ, make_op_lea(1, &ret), temp, "Copy val to new temp to not harm it, also add 1 to get correct index");

			add_2_ins(&head, &tail, ADDQ, v, temp, "Adding index to start of array");

			mem = make_op_mem_loc(&temp);
			add_2_ins(&head, &tail, MOVQ, mem, mem, "Used to get target for next instruction");
			break;

		case (var_RECORD):
			s = get_symbol(var->val.record.var->stype->val.record_type->table, var->val.record.id);

			ret = make_op_temp();
			add_2_ins(&head, &tail, MOVQ, make_op_const(s->offset), ret, "Copy index to new reg");

			variable = generate_var(var->val.record.var);
			asm_insert(&head, &tail, &variable);
			v = get_return_reg(tail);

			//Insert RTC failure label to check if record is initialized
			if (runtime_checks){
				uninit_var_flag = 1;
				make_uninitialized_var_label(label_uninit);
				add_2_ins(&head, &tail, CMP, make_op_const(0), v, "Check if variable is initialized");
				add_1_ins(&head, &tail, JE, make_op_label(label_uninit), "Jump to error label");
			}

			temp = make_op_temp();
			
			add_2_ins(&head, &tail, LEAQ, make_op_lea(1, &ret),  temp, "Copy val to new temp to not harm it, also add 1 to get correct index");
			//printf("Using record var: %s, offset: %d\n", s->name, s->offset);

			add_2_ins(&head, &tail, ADDQ, v,  temp, "Adding index to start of record");

			mem = make_op_mem_loc(&temp);
			add_2_ins(&head, &tail, MOVQ, mem, mem, "Used to get target for next instruction");


			break;



		default:
			break;

	}
	return head;

}

a_asm *generate_exp(expression *exp){

    struct a_asm *left_exp;
    struct a_asm *right_exp;
    struct a_asm *single;
	struct asm_op *left_target;
	struct asm_op *right_target;
	struct asm_op *temp;
	char label_true[20];
	char label_end[20];
	char label_bool[20];

    struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;
    

    if (exp->kind == exp_TERM){

        single = generate_term(exp->val.term);
		asm_insert(&head, &tail, &single);
		return head;
    }

    left_exp = generate_exp(exp->val.ops.left);
    asm_insert(&head, &tail, &left_exp);
	left_target = get_return_reg(tail);
		
	if (exp->kind == exp_OR){
		make_bool_label(label_bool);
		temp = make_op_temp(); //Used to hold the result of the expression
		add_2_ins(&head, &tail, MOVQ, make_op_const(1), temp, "Setting default value of result to true");
		
		add_2_ins(&head, &tail, CMP, make_op_const(1), left_target, "Compare left side of OR with true");
		add_1_ins(&head, &tail, JE, make_op_label(label_bool), "If true, skip right expression");
		
		right_exp = generate_exp(exp->val.ops.right);
		asm_insert(&head, &tail, &right_exp);
		right_target = get_return_reg(tail);

		//If this is excecuted, left expression was false, so the result depends entirely on the right expression
		add_2_ins(&head, &tail, MOVQ, right_target, temp, "Result is set to the value of the right expression");

		add_label(&head, &tail, label_bool, "OR expression label");

		add_2_ins(&head, &tail, MOVQ, temp, temp, "Used to get target for next instruction");
		
		return head;

	}

	if (exp->kind == exp_AND){
		make_bool_label(label_bool);
		temp = make_op_temp();

		add_2_ins(&head, &tail, MOVQ, make_op_const(0), temp, "Setting default value of result to false");
		
		add_2_ins(&head, &tail, CMP, make_op_const(0), left_target, "Compare left side of AND with false");
		add_1_ins(&head, &tail, JE, make_op_label(label_bool), "If true, skip right expression");
		
		right_exp = generate_exp(exp->val.ops.right);
		asm_insert(&head, &tail, &right_exp);
		right_target = get_return_reg(tail);

		//If this is excecuted, left expression was false, so the result depends entirely on the right expression
		add_2_ins(&head, &tail, MOVQ, right_target, temp, "Result is set to the value of the right expression");

		add_label(&head, &tail, label_bool, "AND expression label");

		add_2_ins(&head, &tail, MOVQ, temp, temp, "Used to get target for next instruction");
		
		return head;

	}

	right_exp = generate_exp(exp->val.ops.right);
	asm_insert(&head, &tail, &right_exp);
	right_target = get_return_reg(tail);

	switch (exp->kind){

		case (exp_MULT):
			//TODO should probaly be changed to be the RDX register?
			//IMUL uses the %rax register, so we must move one of the values to this register
			//Put the resulting value in a temporary
			add_2_ins(&head, &tail, MOVQ, left_target, reg_RAX, "Using RAX for multiplication");
			add_2_ins(&head, &tail, MOVQ, right_target, reg_RDX, "Using RDX for multiplication");
			add_1_ins(&head, &tail, IMUL, reg_RDX, "Multiplication using RAX and RDX");
			add_2_ins(&head, &tail, MOVQ, reg_RAX, make_op_temp(), "Storing result here (temp)");
			break;

		case (exp_DIV):
			//IDIV uses the %rax register, so we must move one of the values to this register
			//Should be change to a multiplication when optimizing, since that is faster than division
			//add_1_ins(&head, &tail, PUSH, reg_RDX, "Push RDX, since it will be overwritten from division");

			add_2_ins(&head, &tail, MOVQ, left_target, reg_RAX, "Using RAX for division");
			add_ins(&head, &tail, CDQ, "Sign-extend RAX into RDX");
			
			//Check to see if this value is 0 or not
			add_2_ins(&head, &tail, MOVQ, right_target, reg_RBX, "Using RBX for division");
			if(runtime_checks) {
				div_zero_flag = 1;
				make_div_zero_label(label_true);
				add_2_ins(&head, &tail, CMP, make_op_const(0), reg_RBX, "Checking if value is 0");
				add_1_ins(&head, &tail, JE, make_op_label(label_true), "Jump to error label");
			}

			add_1_ins(&head, &tail, IDIV, reg_RBX, "Dividing RAX with RBX");
			add_2_ins(&head, &tail, MOVQ, reg_RAX, make_op_temp(), "Storing result here (temp)");
			break;

		case (exp_PLUS):
			add_2_ins(&head, &tail, ADDQ, left_target, right_target, "Addition");
			break;
		
		case (exp_MIN):
			add_2_ins(&head, &tail, SUBQ, right_target, left_target, "Subtraction");
			break;

		case (exp_EQ):
			add_2_ins(&head, &tail, CMP, right_target, left_target, "Compare, EQ");
			//Will hold either 1 or 0, depending on if the expression was true of false
			temp = make_op_temp();
			make_cmp_label(label_true);
			make_end_cmp_label(label_end);
			add_1_ins(&head, &tail, JE, make_op_label(label_true), "If true, jump to label");
			
			add_2_ins(&head, &tail, MOVQ, make_op_const(0), temp, "Setting result to 0 (false)");
			add_1_ins(&head, &tail, JMP, make_op_label(label_end), "Jump to after compare label");
			add_label(&head, &tail, label_true, "Compare true label");
			add_2_ins(&head, &tail, MOVQ, make_op_const(1), temp, "Setting result to 1 (true)");
			add_label(&head, &tail, label_end, "After compare label");
			add_2_ins(&head, &tail, MOVQ, temp, temp, "Used to get \"target\" when creating next instruction");
			break;

		case (exp_NEQ):
			add_2_ins(&head, &tail, CMP, right_target, left_target, "Compare, NEQ");
			//Will hold either 1 or 0, depending on if the expression was true of false
			temp = make_op_temp();
			make_cmp_label(label_true);
			make_end_cmp_label(label_end);
			add_1_ins(&head, &tail, JNE, make_op_label(label_true), "If true, jump to label");
			
			add_2_ins(&head, &tail, MOVQ, make_op_const(0), temp, "Setting result to 0 (false)");
			add_1_ins(&head, &tail, JMP, make_op_label(label_end), "Jump to after compare label");
			add_label(&head, &tail, label_true, "Compare true label");
			add_2_ins(&head, &tail, MOVQ, make_op_const(1), temp, "Setting result to 1 (true)");
			add_label(&head, &tail, label_end, "After compare label");
			add_2_ins(&head, &tail, MOVQ, temp, temp, "Used to get \"target\" when creating next instruction");
			break;

		case (exp_GT):
			add_2_ins(&head, &tail, CMP, right_target, left_target, "Compare, GT");
			//Will hold either 1 or 0, depending on if the expression was true of false
			temp = make_op_temp();
			make_cmp_label(label_true);
			make_end_cmp_label(label_end);
			add_1_ins(&head, &tail, JG, make_op_label(label_true), "If true, jump to label");
			
			add_2_ins(&head, &tail, MOVQ, make_op_const(0), temp, "Setting result to 0 (false)");
			add_1_ins(&head, &tail, JMP, make_op_label(label_end), "Jump to after compare label");
			add_label(&head, &tail, label_true, "Compare true label");
			add_2_ins(&head, &tail, MOVQ, make_op_const(1), temp, "Setting result to 1 (true)");
			add_label(&head, &tail, label_end, "After compare label");
			add_2_ins(&head, &tail, MOVQ, temp, temp, "Used to get \"target\" when creating next instruction");
			break;

		case (exp_LT):
			add_2_ins(&head, &tail, CMP, right_target, left_target, "Compare, LT");
			//Will hold either 1 or 0, depending on if the expression was true of false
			temp = make_op_temp();
			make_cmp_label(label_true);
			make_end_cmp_label(label_end);
			add_1_ins(&head, &tail, JL, make_op_label(label_true), "If true, jump to label");
			
			add_2_ins(&head, &tail, MOVQ, make_op_const(0), temp, "Setting result to 0 (false)");
			add_1_ins(&head, &tail, JMP, make_op_label(label_end), "Jump to after compare label");
			add_label(&head, &tail, label_true, "Compare true label");
			add_2_ins(&head, &tail, MOVQ, make_op_const(1), temp, "Setting result to 1 (true)");
			add_label(&head, &tail, label_end, "After compare label");
			add_2_ins(&head, &tail, MOVQ, temp, temp, "Used to get \"target\" when creating next instruction");
			break;

		case (exp_GEQ):
			add_2_ins(&head, &tail, CMP, right_target, left_target, "Compare, GEQ");
			//Will hold either 1 or 0, depending on if the expression was true of false
			temp = make_op_temp();
			make_cmp_label(label_true);
			make_end_cmp_label(label_end);
			add_1_ins(&head, &tail, JGE, make_op_label(label_true), "If true, jump to label");
			
			add_2_ins(&head, &tail, MOVQ, make_op_const(0), temp, "Setting result to 0 (false)");
			add_1_ins(&head, &tail, JMP, make_op_label(label_end), "Jump to after compare label");
			add_label(&head, &tail, label_true, "Compare true label");
			add_2_ins(&head, &tail, MOVQ, make_op_const(1), temp, "Setting result to 1 (true)");
			add_label(&head, &tail, label_end, "After compare label");
			add_2_ins(&head, &tail, MOVQ, temp, temp, "Used to get \"target\" when creating next instruction");
			break;
		
		case (exp_LEQ):
			add_2_ins(&head, &tail, CMP, right_target, left_target, "Compare, LEQ");
			//Will hold either 1 or 0, depending on if the expression was true of false
			temp = make_op_temp();
			make_cmp_label(label_true);
			make_end_cmp_label(label_end);
			add_1_ins(&head, &tail, JLE, make_op_label(label_true), "If true, jump to label");
			
			add_2_ins(&head, &tail, MOVQ, make_op_const(0), temp, "Setting result to 0 (false)");
			add_1_ins(&head, &tail, JMP, make_op_label(label_end), "Jump to after compare label");
			add_label(&head, &tail, label_true, "Compare true label");
			add_2_ins(&head, &tail, MOVQ, make_op_const(1), temp, "Setting result to 1 (true)");
			add_label(&head, &tail, label_end, "After compare label");
			add_2_ins(&head, &tail, MOVQ, temp, temp, "Used to get \"target\" when creating next instruction");
			break;

		default:
			break;




	}

    return head;
    
}

a_asm *generate_term(term *term){
	struct a_asm *head;
    struct a_asm *tail;
	struct a_asm *v;
	head = NULL;
	tail = NULL;

	SYMBOL *s;

	struct asm_op *target;
	struct asm_op *reg;
	struct asm_op *static_link;
	struct asm_op *temp;
	struct asm_op *temp2;
	struct asm_op *res;
	struct asm_op *mem;

	struct a_asm *t;
	struct a_asm *el;
	struct a_asm *expr;
	struct exp_list *elist;
	int arg_count;
	int used_reg;
	int depth;
	arg_count = 0;

    switch (term->kind){

		case (term_VAR):
			v = generate_var(term->val.variable);
			asm_insert(&head, &tail, &v);
			target = get_return_reg(tail);
			add_2_ins(&head, &tail, MOVQ, target, make_op_temp(), "Copy val to new temp, to not harm it");
			break;

		case (term_LIST):

			add_ins(&head, &tail, BEGIN_CALL, "Beginning of function call");
			s = get_symbol(term->table, term->val.list.id);
			//printf("Calling function: %s, args: %d\n", s->name, s->stype->val.func_type.func->head->args);
			if (term->val.list.list->kind != al_EMPTY){

				//If more than "PLACE_IN_REGS" parameter, place parameters on stack
				if (s->stype->val.func_type.func->head->args > PLACE_IN_REGS){

					el = generate_elist(term->val.list.list->list, &arg_count);

					asm_insert(&head, &tail, &el);
					//printf("Args pushed: %d\n", arg_count);
				} else {
					elist = term->val.list.list->list;
					used_reg = AVAIL_REGS-1;
					while (elist != NULL){

						reg = get_corresponding_reg(used_reg);
						expr = generate_exp(elist->expression);
						asm_insert(&head, &tail, &expr);
						target = get_return_reg(tail);
						add_2_ins(&head, &tail, MOVQ, target, reg, "Moving function parameter to register");
						arg_count++;
						used_reg--;

						if (elist->kind == el_LIST){
							elist = elist->list;
						} else {
							elist = NULL;
						}

					}

				}
			

			}
			//Set up static link
			depth = get_symbol_depth(term->table, term->val.list.id);
			static_link = make_op_temp();
			
			if (depth == 0){
				//printf("Non-recursive function: %s, depth: %d\n", term->val.list.id, depth);

				add_2_ins(&head, &tail, MOVQ, reg_RBP, static_link, "Setting address wanted for static link");
				add_1_ins(&head, &tail, PUSH, static_link, "Storing static link for function");

			} else {
				//printf("Possibly recursive function: %s, depth: %d\n", term->val.list.id, depth);
				
				add_2_ins(&head, &tail, MOVQ, op_STATIC_LINK, static_link, "Retrieving static link");
				
				//Resolve static link
				depth--;

				while (depth > 0){
					temp = make_op_stack_loc(0, &static_link);
					static_link = make_op_temp();
					add_2_ins(&head, &tail, MOVQ, temp, static_link, "Copying static link");
					depth--;

				}

				add_1_ins(&head, &tail, PUSH, static_link, "Storing static link for function");

			}

			add_1_ins(&head, &tail, CALL, make_op_label(s->name), "Calling function");
			tail->func_args = arg_count;

			if (s->stype->val.func_type.func->head->args > PLACE_IN_REGS){

				add_2_ins(&head, &tail, ADDQ, make_op_const((arg_count+1)*8), reg_RSP, "Removing arguments and static link");

			} else {
				add_2_ins(&head, &tail, ADDQ, make_op_const(8), reg_RSP, "Remove static link");
			}

			add_2_ins(&head, &tail, MOVQ, reg_RAX, make_op_temp(), "Saving return value from function in temp");

			add_ins(&head, &tail, END_CALL, "End of function call");

			break;
		
		case (term_PAR):
			expr = generate_exp(term->val.expression);
			asm_insert(&head, &tail, &expr);
			break;

		case (term_ABS):
			expr = generate_exp(term->val.expression);
			asm_insert(&head, &tail, &expr);
			if (term->val.expression->stype->type == symbol_INT){

				target = get_return_reg(tail);
				temp = make_op_temp();
				add_2_ins(&head, &tail, MOVQ, target, temp, "Moving argument to new reg");
				add_2_ins(&head, &tail, MOVQ, temp, reg_RAX, "Move val into RAX before CDQ");

				add_ins(&head, &tail, CDQ, "Sign-extend RAX into RDX");

				temp2 = make_op_temp();
				add_2_ins(&head, &tail, MOVQ, reg_RDX, temp2, "Move val to new reg");
				
				add_2_ins(&head, &tail, XORQ, temp, temp2, "XOR vals");

				add_2_ins(&head, &tail, SUBQ, reg_RDX, temp2, "Subtraction");

				add_2_ins(&head, &tail, MOVQ, temp2, temp2, "Used to get target for next instruction");

			}
			if (term->val.expression->stype->type == symbol_ARRAY){
				target = get_return_reg(tail);
				
				temp = make_op_temp();
				add_2_ins(&head, &tail, MOVQ, make_op_const(0), temp, "Want to get value at index 0 of array");

				add_2_ins(&head, &tail, ADDQ, target, temp, "Get value at index 0");
				

				mem = make_op_mem_loc(&temp);
				add_2_ins(&head, &tail, MOVQ, mem, mem, "Used to get target for next instruction");
			}

			break;

		case (term_NOT):
			t = generate_term(term->val.term_not);
			asm_insert(&head, &tail, &t);
			target = get_return_reg(tail);

			add_2_ins(&head, &tail, XORQ, make_op_const(1), target, "XOR with 1 will give the NOT of the term");

			break;

			




        case (term_NUM):
			add_2_ins(&head, &tail, MOVQ, make_op_const(term->val.num), make_op_temp(), "Moving constant to register");
			break;

		case (term_TRUE):
			add_2_ins(&head, &tail, MOVQ, make_op_const(1), make_op_temp(), "Moving TRUE to register");
			break;

		case (term_FALSE):
			add_2_ins(&head, &tail, MOVQ, make_op_const(0), make_op_temp(), "Moving FALSE to register");
			break;

		case (term_NULL):
			add_2_ins(&head, &tail, MOVQ, make_op_const(0), make_op_temp(), "Moving NULL to register");
			break;

		default:
			break;




    }

	return head;
    
}

a_asm *generate_elist(exp_list *elist, int *count){
	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;

	struct a_asm *el;
	struct a_asm *expr;
	struct asm_op *target;


	if (elist->kind == el_LIST){
		el = generate_elist(elist->list, count);
		asm_insert(&head, &tail, &el);
	}

	expr = generate_exp(elist->expression);
	asm_insert(&head, &tail, &expr);
	target = get_return_reg(tail);
	add_1_ins(&head, &tail, PUSH, target, "Push argument for function");
	(*count)++;

    

	return head;
}

// Insert a "linked list" into an existing list
void asm_insert(a_asm **head, a_asm **tail, a_asm **new){
    struct a_asm *temp;
	struct a_asm *temp2;
	temp = NULL;

	if ((*tail) == NULL){
		nullInserts++;
	} else {
		nonNullInserts++;
	}

    
    if (new != NULL && (*new) != NULL){
		if (*tail != NULL){
			temp = (*tail)->next;
			(*tail)->next = (*new);
    	}

        (*new)->prev = *tail;
        if ((*tail) == NULL){
            *head = (*new);
        }

		(*tail) = find_tail((*new));

		(*tail)->next = temp;
        if (temp != NULL){
			temp->prev = (*tail);
		}

    }

}

void asm_insert_one(a_asm **head, a_asm **tail, a_asm **next){
	if ((*head) == NULL){
		*head = *next;
		*tail = *next;

	} else {
		(*next)->prev = *tail;
		(*tail)->next = *next;
		(*tail) = *next;
	}
}

//Add instruction with 2 operators
void add_2_ins(a_asm **head, a_asm **tail, ASM_kind ins, asm_op *op1, asm_op *op2, char* comment){
	get_next(head, tail);
	
	(*tail)->ins = ins;
	(*tail)->val.two_op.op1 = op1;
	(*tail)->val.two_op.op2 = op2;
	(*tail)->comment = comment;
	(*tail)->ops = 2;
	(*tail)->next = NULL;

}

//Add instruction with 1 operator
void add_1_ins(a_asm **head, a_asm **tail, ASM_kind ins, asm_op *op1, char* comment){
	get_next(head, tail);
	
	(*tail)->ins = ins;
	(*tail)->val.one_op.op = op1;
	(*tail)->comment = comment;
	(*tail)->ops = 1;
	(*tail)->next = NULL;

}

void add_ins(a_asm **head, a_asm **tail, ASM_kind ins, char *comment){
	get_next(head, tail);

	(*tail)->ins = ins;
	(*tail)->comment = comment;
	(*tail)->ops = 0;
	(*tail)->next = NULL;
}

void add_label(a_asm **head, a_asm **tail, char *label, char *comment){
	get_next(head, tail);

	(*tail)->ins = LABEL;
	(*tail)->val.label.label_id = malloc(sizeof(char) *20);
	(*tail)->val.label.type = label_NORMAL;
	(*tail)->val.label.func_vars = 0;
	
	sprintf((*tail)->val.label.label_id, "%s", label);
	(*tail)->comment = comment;
	(*tail)->next = NULL;

}

void add_func_label(a_asm **head, a_asm **tail, char *label, char *comment, int vars){
	get_next(head, tail);

	(*tail)->ins = LABEL;
	(*tail)->val.label.label_id = malloc(sizeof(char) *20);
	(*tail)->val.label.type = label_FUNC_START;
	(*tail)->val.label.func_vars = vars;
	
	sprintf((*tail)->val.label.label_id, "%s", label);
	(*tail)->comment = comment;
	(*tail)->next = NULL;
}


void get_next(a_asm **head1, a_asm **tail1){
	if (*tail1 == NULL){
		*tail1 = NEW(a_asm);
		if (tail1 == NULL){
		}
		*head1 = *tail1;
	} else {
		(*tail1)->next = NEW(a_asm);
		(*tail1)->next->prev = *tail1;
		*tail1 = (*tail1)->next;
	}

}

asm_op *get_return_reg(a_asm *tail){
	if (tail->ins == END_CALL){
		return tail->prev->val.two_op.op2;
	}
	return tail->val.two_op.op2;
}

//Assign temp to every local variable
int local_init(decl_list *dlist){
	struct decl_list *d_temp;
	struct var_decl_list *v_temp;
	SYMBOL *s;
	int vars;
	int contains_function;
	int offset;
	vars = 0;
	contains_function = 0;
	offset = 0;

	d_temp = dlist;

	while (d_temp->kind != dl_EMPTY){
		if (d_temp->decl->kind == decl_FUNC){
			//printf("Declaration contains a function\n");
			contains_function = 1;
			break;
		}
		d_temp = d_temp->list;

	}

	d_temp = dlist;

	//If the program contains functions, put variables on the stack, so we can reach them with static link
	if (contains_function){
		while (d_temp->kind != dl_EMPTY){
			if (d_temp->decl->kind == decl_VAR){
				v_temp = d_temp->decl->val.list;
				while (v_temp != NULL){
					s = get_symbol(dlist->table, v_temp->vartype->id);
					//printf("Setting offset of %s to %d\n", s->name, offset);
					s->offset = offset;
					s->is_on_stack = 1;
					offset++;
					v_temp = v_temp->list;
				}
			}
			d_temp = d_temp->list;
		}
		return offset;
	}

	//If no functions, we can just keep the variables in registers/temps
	while (d_temp->kind != dl_EMPTY){
		if (d_temp->decl->kind == decl_VAR){
			v_temp = d_temp->decl->val.list;
			while (v_temp != NULL){
				s = get_symbol(dlist->table, v_temp->vartype->id);

				//For some reason memory location using registers did not work, so force it to use stack
				if (s->stype->type == symbol_ARRAY || s->stype->type == symbol_RECORD){
					s->offset = offset;
					s->is_on_stack = 1;
					offset++;
				} else {

					s->op = make_op_temp();
					s->is_on_stack = 0;
				}
				
				v_temp = v_temp->list;
				vars++;
			}
		}
		d_temp = d_temp->list;
	}
	return vars;
}


//Should not be necessary if we keep meta information on blocks
a_asm *find_tail(a_asm *node){

	struct a_asm *temp;
	temp = node;
	int i = 0;

	while (temp->next != NULL){
		
		temp = temp->next;
		
	}

	return temp;
	
}


asm_op *make_op_const(int i){
	struct asm_op *op;
	op = NEW(asm_op);
	
	op->type = op_INTCONST;
	op->val.intconst = i;
	return op;

}

asm_op *make_op_temp(){
	struct asm_op *op;
	op = NEW(asm_op);

	op->type = op_TEMP;
	op->val.temp.id = temps;
	op->val.temp.spill = NULL;
	temps++;
	return op;

}


asm_op *make_op_label(char *label){
	struct asm_op *op;
	op = NEW(asm_op);

	op->type = op_LABEL;
	op->val.label_id = malloc(sizeof(char) * 20);
	sprintf(op->val.label_id, "%s", label);
	return op;

}


asm_op *make_op_spill(){
	struct asm_op *op;
	op = NEW(asm_op);

	op->type = op_SPILL;
	op->stack_offset = -1;

	return op;

}


void make_cmp_label(char *buffer){
	
	cmps++;
	sprintf(buffer, "cmpTrue_%d", cmps);
	

}


void make_end_cmp_label(char *buffer){

	sprintf(buffer, "endCMP_%d", cmps);
	
}

void make_bool_label(char *buffer){

	cmps++;
	sprintf(buffer, "endBoolCMP_%d", cmps);
	
}


void make_else_label(char *buffer){
	
	ifs++;
	sprintf(buffer, "else_%d", ifs);

}


void make_if_label(char *buffer){
	

	sprintf(buffer, "if_end_%d", ifs);

}

void make_loop_start_label(char *buffer){
	loops++;
	sprintf(buffer, "loop_start_%d", loops);


}

void make_loop_end_label(char *buffer){
	sprintf(buffer, "loop_end_%d", loops);

}

void make_div_zero_label(char *buffer) {
	sprintf(buffer, "div_zero");
}
void make_array_index_label(char *buffer){
	sprintf(buffer, "array_index");
}

void make_positive_allocate_label(char *buffer){
	sprintf(buffer, "positive_allocate");
}

void make_uninitialized_var_label(char *buffer){
	sprintf(buffer, "uninit_var");
}

void make_out_of_mem_label(char *buffer){
	sprintf(buffer, "out_of_mem");
}

asm_op *make_op_mem_loc(asm_op **index_reg){

	struct asm_op *op;
	op = NEW(asm_op);

	op->type = op_MEM_LOC;
	op->val.mem_index_reg = (*index_reg);

	return op;
}

asm_op *make_op_stack_loc(int offset, asm_op **reg){

	struct asm_op *op;
	op = NEW(asm_op);

	if (reg != reg_RBP){
		op->type = op_STACK_LOC;
		op->val.stack.reg = (*reg);
		op->val.stack.offset = offset;
		return op;
	}

	op->type = op_LABEL;
	op->val.label_id = malloc(sizeof(char) *20);
	sprintf(op->val.label_id, "%d(%%rbp)", -8 * offset);
	op->stack_offset = offset;

	return op;

}

//LEA op with one register
asm_op *make_op_lea(int offset, asm_op **reg){

	struct asm_op *op;
	op = NEW(asm_op);

	op->type = op_LEA;
	op->val.lea.reg = (*reg);
	op->val.lea.offset = offset;

	return op;
}

// Runtime errors


void add_array_index_runtime_error(a_asm **head, a_asm **tail) {
	add_label(head, tail, "array_index", "Add array index runtime check");
	add_2_ins(head, tail, MOVQ, make_op_const(1), reg_RAX, "Interrupt code");
	add_2_ins(head, tail, MOVQ, make_op_const(2), reg_RBX, "Set return code");
	add_1_ins(head, tail, INT_, make_op_label("$0x80"), "Call exit");
}

void add_zero_div_runtime_error(a_asm **head, a_asm **tail) {
	add_label(head, tail, "div_zero", "Add division by zero runtime check");
	add_2_ins(head, tail, MOVQ, make_op_const(1), reg_RAX, "Interrupt code");
	add_2_ins(head, tail, MOVQ, make_op_const(3), reg_RBX, "Set return code");
	add_1_ins(head, tail, INT_, make_op_label("$0x80"), "Call exit");
}

void add_positive_allocate_runtime_error(a_asm **head, a_asm **tail) {
	add_label(head, tail, "positive_allocate", "Add positive allocate runtime check");
	add_2_ins(head, tail, MOVQ, make_op_const(1), reg_RAX, "Interrupt code");
	add_2_ins(head, tail, MOVQ, make_op_const(4), reg_RBX, "Set return code");
	add_1_ins(head, tail, INT_, make_op_label("$0x80"), "Call exit");
}

void add_uninitialized_var_runtime_error(a_asm **head, a_asm **tail) {
	add_label(head, tail, "uninit_var", "Add uninitialized variable runtime check");
	add_2_ins(head, tail, MOVQ, make_op_const(1), reg_RAX, "Interrupt code");
	add_2_ins(head, tail, MOVQ, make_op_const(5), reg_RBX, "Set return code");
	add_1_ins(head, tail, INT_, make_op_label("$0x80"), "Call exit");
}

void add_out_of_mem_runtime_error(a_asm **head, a_asm **tail) {
	add_label(head, tail, "out_of_mem", "Add out of memory runtime check");
	add_2_ins(head, tail, MOVQ, make_op_const(1), reg_RAX, "Interrupt code");
	add_2_ins(head, tail, MOVQ, make_op_const(6), reg_RBX, "Set return code");
	add_1_ins(head, tail, INT_, make_op_label("$0x80"), "Call exit");
}

void add_runtime_checks(a_asm **head, a_asm **tail){
	if (array_index_flag){
		add_array_index_runtime_error(head, tail);
	}

	if (div_zero_flag){
		add_zero_div_runtime_error(head, tail);
	}

	if (positive_allocate_flag){
		add_positive_allocate_runtime_error(head, tail);
	}

	if (uninit_var_flag){
		add_uninitialized_var_runtime_error(head, tail);
	}

	if (out_of_mem_flag){
		add_out_of_mem_runtime_error(head, tail);
	}

}

a_asm *add_zero_init(decl_list *dlist){
	struct a_asm *head;
	struct a_asm *tail;

	struct decl_list *d_temp;
	struct var_decl_list *v_temp;

	struct SYMBOL *s;

	head = NULL;
	tail = NULL;

	d_temp = dlist;

	

	while (d_temp->kind != dl_EMPTY){
		if (d_temp->decl->kind == decl_VAR){
			v_temp = d_temp->decl->val.list;
			while (v_temp != NULL){
				s = get_symbol(dlist->table, v_temp->vartype->id);

				if (s->is_on_stack){
					add_2_ins(&head, &tail, MOVQ, make_op_const(0), make_op_stack_loc(s->offset, reg_RBP), "Init to zero");
				} else {
					add_2_ins(&head, &tail, MOVQ, make_op_const(0), s->op, "Init to zero");
				}
				
				v_temp = v_temp->list;
				
			}
		}
		d_temp = d_temp->list;
	}

	return head;

}