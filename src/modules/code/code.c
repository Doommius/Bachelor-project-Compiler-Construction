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

int temps = AVAIL_REGS; //The predefined registers will be the first values in the bitvectors
int cmps = 1;
int ifs = 1;
int loops = 1;
int nullInserts = 0;
int nonNullInserts = 0; 

/**
 * TODO here
 * Have not yet implemented pushes of RBP and RSP and such, before calling functions
 * A possibility for doing this would be to add an instruction with a special type "FUNC_START" and "FUNC_END" or such
 * Where we replace the "FUNC_START" with instructions to push the correct registers and such
 * And in "FUNC_END" we restore those registers.
 * 
 */


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
	wrt_INT->val.label_id = ".wrt_INT";

	op_PRINTF = NEW(asm_op);
	op_PRINTF->type = op_LABEL;
	op_PRINTF->val.func_id = "printf";

}

a_asm *generate_program(body *body){
	struct a_asm *b;
	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;
    
	printf("Initializing registers\n");

	init_regs();


	printf("Generating body\n");
	b = generate_body(body, "main", "main_end");

	asm_insert(&head, &tail, &b);
	printf("Done generating program\n");
	add_2_ins(&head, &tail, MOVQ, make_op_const(0), reg_RAX, "Return \"no error\" exit code");
	add_ins(&head, &tail, RET, "Program return");

	printf("Null tail inserts: %d\n", nullInserts);
	printf("Non-Null tail inserts: %d\n", nonNullInserts);


	return head;
   
}

a_asm *generate_body(body *body, char *start_label, char *end_label){
	int vars;
	int tmps = 0;

	struct a_asm *sl;
	struct a_asm *dl;
	struct a_asm *head;
    struct a_asm *tail;
	struct a_asm *head2;
	struct a_asm *tail2;
	struct a_asm *temp;
	head = NULL;
	tail = NULL;
	head2 = NULL;
	tail2 = NULL;
	
	vars = local_init(body->d_list);
	printf("Vars: %d, in function: %s\n", vars, start_label);


	printf("In body, generating d_list\n");
	dl = generate_dlist(body->d_list);
	asm_insert(&head, &tail, &dl);
	tmps = temps;
	printf("In body, done generating d_list, created %d temps for now in body: %s\n", tmps, start_label);

	//We want "main" to be the last label in the file.
	add_label(&head, &tail, start_label, "Start of body");

	printf("In body, generating s_list\n");
	sl = generate_slist(body->s_list);
	asm_insert(&head2, &tail2, &sl);

	
	tmps = temps - tmps;
	printf("In body, done generating s_list\n");


	//This is not nice
	temp = NEW(a_asm);
	temp->comment = "Reserving space for temps in body";
	temp->ins = SUBQ;
	printf("Reserving space for %d temps in body of: %s\n", tmps, start_label);
	temp->val.two_op.op1 = make_op_const(8 * tmps);
	temp->val.two_op.op2 = reg_RSP;
	
	//add_simple_start(&head, &tail);

	//asm_insert(&head, &tail, &temp);

	asm_insert(&head, &tail, &head2);



	add_label(&head, &tail, end_label, "End of body");
	//add_simple_end(&head, &tail);

	return head;
	
}

a_asm *generate_function(function *func){
	struct a_asm *head;
    struct a_asm *tail;
	struct a_asm *b;
	struct a_asm *h;
	head = NULL;
	tail = NULL;

	h = generate_head(func->head);
	asm_insert(&head, &tail, &h);

	b = generate_body(func->body, func->start_label, func->end_label);
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

	pdl = generate_pdl(h->list);
	asm_insert(&head, &tail, &pdl);

	return head;
}

a_asm *generate_pdl(par_decl_list *pdl){
	struct a_asm *head;
    struct a_asm *tail;
	struct a_asm *vdl;
	int offset;
	head = NULL;
	tail = NULL;
    
	if (pdl->kind != pdl_EMPTY){
		offset = 1;
		vdl = generate_vdl(pdl->list, offset);
		asm_insert(&head, &tail, &vdl);
	}

	return head;
}

a_asm *generate_vdl(var_decl_list *vdl, int offset){
	struct a_asm *head;
    struct a_asm *tail;
	struct a_asm *l;
	SYMBOL *s;
	head = NULL;
	tail = NULL;

	s = get_symbol(vdl->vartype->table, vdl->vartype->id);

	//Possibly -8 instead of 8
	s->offset = 8 * offset;
	offset++;
	printf("Offset of var: %s = %d\n", s->name, s->offset);
	if ( vdl->kind == vdl_LIST && vdl->list != NULL){
		l = generate_vdl(vdl->list, offset);
		asm_insert(&head, &tail, &l);
	}
    
	return head;
}

a_asm *generate_vtype(var_type *vtype){
	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;
    

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


	stmt = generate_stmt(slist->statement);
	
	asm_insert(&head, &tail, &stmt);


	if (slist->kind !=NULL){
		
		sl = generate_slist(slist->list);
		asm_insert(&head, &tail, &sl);
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

	head = NULL;
	tail = NULL;
	struct a_asm *expr;
	struct a_asm *s1;
	struct a_asm *s2;
	struct a_asm *v;

	struct asm_op *wrt;
	struct asm_op *ret;
	struct asm_op *ifexp;
	struct asm_op *var;
	struct asm_op *assignexp;


	switch (stmt->kind){

		case (statement_RETURN):
			expr = generate_exp(stmt->val.ret);
			asm_insert(&head, &tail, &expr);
			ret = tail->val.two_op.op2;
			add_2_ins(&head, &tail, MOVQ, ret, reg_RAX, "Return value placed in RAX");
			add_1_ins(&head, &tail, JMP, make_op_label(stmt->function->end_label), "Jump to functions end label");
			break;

		case (statement_WRITE):

			expr = generate_exp(stmt->val.wrt);
			asm_insert(&head, &tail, &expr);
			wrt = tail->val.two_op.op2;
			//Should probably be some more pushing before creating a print
			add_1_ins(&head, &tail, PUSH, reg_RAX, "Saving value of RAX before printf call");

			add_2_ins(&head, &tail, MOVQ, wrt_INT, reg_RDI, "First argument for printf");
			add_2_ins(&head, &tail, MOVQ, wrt, reg_RSI, "Second argument for printf");
			add_2_ins(&head, &tail, MOVQ, make_op_const(0), reg_RAX, "No vector arguments");
			add_1_ins(&head, &tail, CALL, op_PRINTF, "Calling printf");

			add_1_ins(&head, &tail, POP, reg_RAX, "Restoring RAX");

			break;

		case (statement_IF):
			expr = generate_exp(stmt->val.ifthen.expression);
			asm_insert(&head, &tail, &expr);
			ifexp = tail->val.two_op.op2;
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
			ifexp = tail->val.two_op.op2;
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
			assignexp = tail->val.two_op.op2;

			v = generate_var(stmt->val.assignment.variable);
			asm_insert(&head, &tail, &v);
			var = tail->val.two_op.op2;


			add_2_ins(&head, &tail, MOVQ, assignexp, var, "Assigning value to var");
			break;

		case (statement_WHILE):
			
			make_loop_start_label(label_loop_start);
			make_loop_end_label(label_loop_end);
			add_label(&head, &tail, label_loop_start, "Start of while");
			expr = generate_exp(stmt->val.loop.expression);
			asm_insert(&head, &tail, &expr);
			ifexp = tail->val.two_op.op2;

			add_2_ins(&head, &tail, CMP, make_op_const(1), ifexp, "Check if condition in while is true");
			add_1_ins(&head, &tail, JNE, make_op_label(label_loop_end), "If condition is false, jump to end");
			s1 = generate_stmt(stmt->val.loop.statement);
			asm_insert(&head, &tail, &s1);
			add_1_ins(&head, &tail, JMP, make_op_label(label_loop_start), "Jump to start of while");
			add_label(&head, &tail, label_loop_end, "End of while");
			break;






		default:
			break;

	}
    
	return head;
}


a_asm *generate_var(variable *var){
	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;

	struct asm_op *v;
	struct asm_op *temp;

	SYMBOL *s;

	switch (var->kind){

		case (var_ID):
			s = get_symbol(var->table, var->id);
			if (s->offset == 0){
				v = s->op;
				add_2_ins(&head, &tail, MOVQ, v, v, "Used to get target for next instruction");
			} else {
				v = make_op_stack_loc(s->offset);
				add_2_ins(&head, &tail, MOVQ, v, v, "Used to get target for next instruction");

			}
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
	left_target = tail->val.two_op.op2;
	
	if (exp->kind == exp_OR){
		make_bool_label(label_bool);
		temp = make_op_temp(); //Used to hold the result of the expression
		add_2_ins(&head, &tail, MOVQ, make_op_const(1), temp, "Setting default value of result to true");
		
		add_2_ins(&head, &tail, CMP, make_op_const(1), left_target, "Compare left side of OR with true");
		add_1_ins(&head, &tail, JE, make_op_label(label_bool), "If true, skip right expression");
		
		right_exp = generate_exp(exp->val.ops.right);
		asm_insert(&head, &tail, &right_exp);
		right_target = tail->val.two_op.op2;

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
		right_target = tail->val.two_op.op2;

		//If this is excecuted, left expression was false, so the result depends entirely on the right expression
		add_2_ins(&head, &tail, MOVQ, right_target, temp, "Result is set to the value of the right expression");

		add_label(&head, &tail, label_bool, "AND expression label");

		add_2_ins(&head, &tail, MOVQ, temp, temp, "Used to get target for next instruction");
		
		return head;

	}

	right_exp = generate_exp(exp->val.ops.right);
	asm_insert(&head, &tail, &right_exp);
	right_target = tail->val.two_op.op2;

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
			add_1_ins(&head, &tail, PUSH, reg_RDX, "Push RDX, since it will be overwritten from division");

			add_2_ins(&head, &tail, MOVQ, left_target, reg_RAX, "Using RAX for division");
			add_ins(&head, &tail, CDQ, "Sign-extend RAX into RDX");

			//Need to add a check to see if this value is 0 or not
			add_2_ins(&head, &tail, MOVQ, right_target, reg_RBX, "Using RBX for division");
			add_1_ins(&head, &tail, IDIV, reg_RBX, "Division using RAX and RBX");
			add_1_ins(&head, &tail, POP, reg_RDX, "Restoring RDX");
			add_2_ins(&head, &tail, MOVQ, reg_RAX, make_op_temp(), "Storing result here (temp)");
			break;

		case (exp_PLUS):
			add_2_ins(&head, &tail, ADDQ, left_target, right_target, "Addition");
			break;

		case (exp_MIN):
			add_2_ins(&head, &tail, SUBQ, left_target, right_target, "Subtraction");
			break;

		case (exp_EQ):
			add_2_ins(&head, &tail, CMP, left_target, right_target, "Compare, EQ");
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
			add_2_ins(&head, &tail, CMP, left_target, right_target, "Compare, NEQ");
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
			add_2_ins(&head, &tail, CMP, left_target, right_target, "Compare, GT");
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
			add_2_ins(&head, &tail, CMP, left_target, right_target, "Compare, LT");
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
			add_2_ins(&head, &tail, CMP, left_target, right_target, "Compare, GEQ");
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
			add_2_ins(&head, &tail, CMP, left_target, right_target, "Compare, LEQ");
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
	struct a_asm *el;

    switch (term->kind){

		case (term_VAR):
			v = generate_var(term->val.variable);
			asm_insert(&head, &tail, &v);
			target = v->val.two_op.op2;
			add_2_ins(&head, &tail, MOVQ, target, make_op_temp(), "Copy val to new temp, to not harm it");
			break;

		case (term_LIST):
			s = get_symbol(term->table, term->val.list.id);
			//printf("Calling function: %s, args: %d\n", s->name, s->stype->val.func_type.func->head->args);
			if (term->val.list.list->kind != al_EMPTY){
				el = generate_elist(term->val.list.list->list);
				asm_insert(&head, &tail, &el);

			}
			add_1_ins(&head, &tail, CALL, make_op_label(s->name), "Calling function");
			add_2_ins(&head, &tail, MOVQ, reg_RAX, make_op_temp(), "Saving return value from function in temp");

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

		default:
			break;




    }

	return head;
    
}

a_asm *generate_alist(act_list *alist){
	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;
    

	return head;
}

a_asm *generate_elist(exp_list *elist){
	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;

	struct a_asm *el;
	struct a_asm *expr;
	struct asm_op *target;


	if (elist->kind == el_LIST){
		el = generate_elist(elist->list);
		asm_insert(&head, &tail, &el);
	}

	expr = generate_exp(elist->expression);
	asm_insert(&head, &tail, &expr);
	target = tail->val.two_op.op2;
	add_1_ins(&head, &tail, PUSH, target, "Push argument for function");

    

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

    
    if ((*new) != NULL){
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
	(*tail)->val.label_id = malloc(sizeof(char) *20);
	sprintf((*tail)->val.label_id, "%s", label);
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

//Assign temp to every local variable
int local_init(decl_list *dlist){
	struct decl_list *d_temp;
	struct var_decl_list *v_temp;
	SYMBOL *s;
	int vars;
	vars = 0;

	d_temp = dlist;

	while(d_temp->kind != dl_EMPTY){
		if (d_temp->decl->kind == decl_VAR){
			v_temp = d_temp->decl->val.list;
			while (v_temp != NULL){
				s = get_symbol(dlist->table, v_temp->vartype->id);
				s->op = make_op_temp();
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


void make_cmp_label(char *buffer){

	sprintf(buffer, "cmpTrue_%d", cmps);
	cmps++;

}


void make_end_cmp_label(char *buffer){

	sprintf(buffer, "endCMP_%d", cmps-1);
	
}

void make_bool_label(char *buffer){

	sprintf(buffer, "endBoolCMP_%d", cmps);
	cmps++;
}


void make_else_label(char *buffer){

	sprintf(buffer, "else_%d", ifs);
	ifs++;

}


void make_if_label(char *buffer){

	sprintf(buffer, "if_end_%d", ifs-1);

}

void make_loop_start_label(char *buffer){

	sprintf(buffer, "loop_start_%d", loops);
	loops++;

}

void make_loop_end_label(char *buffer){
	sprintf(buffer, "loop_end_%d", loops-1);

}

asm_op *make_op_stack_loc(int offset){

	struct asm_op *op;
	op = NEW(asm_op);

	//Not exactly a label, but it fits with the printing
	op->type = op_LABEL;
	op->val.label_id = malloc(sizeof(char) *20);
	sprintf(op->val.label_id, "%d(%%rbp)", offset);
	op->stack_offset = offset;
	return op;

}


void add_simple_start(a_asm **head, a_asm **tail){
	add_1_ins(head, tail, PUSH, reg_RBP, "Pushing base pointer");
	add_2_ins(head, tail, MOVQ, reg_RSP, reg_RBP, "Making stack pointer new base pointer");

}

void add_simple_end(a_asm **head, a_asm **tail){

	add_2_ins(head, tail, MOVQ, reg_RBP, reg_RSP, "Restoring stack pointer");
	add_1_ins(head, tail, POP, reg_RBP, "Restoring base pointer");

}

unsigned get_temps(){
	return temps;
}