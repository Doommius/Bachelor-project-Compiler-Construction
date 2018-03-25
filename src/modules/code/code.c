/**
 * @brief Assembly code generator for the x86 architecture.
 * 
 * @file code.c
 * @author Morten Jæger, Mark Jervelund & Troels Blicher Petersen
 * @date 2018-03-09
 */

#include <string.h>
#include "tree.h"
#include "linked_list.h"
#include "tree.h"
#include "code.h"
#include "memory.h"

int temps = 0;


/**
 * Initialize general purpose register operators
 * 
 * Also initialize "wrt_INT" and "op_PRINTF", for printing ints
 * 
 */
void init_regs(){
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

void code_or_expression(linked_list *list, expression *e) {
    eval_expression(list, e->val.ops.left);
    linked_list_insert_tail(list, new_asm_node(e->lineno, push, "%eax", "", "", "save value of e1 on the stack"));
    eval_expression(list, e->val.ops.left);
    linked_list_insert_tail(list, new_asm_node(e->lineno, pop, "%ecx", "", "", "pop e1 from the stack into ecx"));
    linked_list_insert_tail(list, new_asm_node(e->lineno, orl, "%ecx", "%eax", "", "compute e1 | e2, set ZF"));
    linked_list_insert_tail(list, new_asm_node(e->lineno, movl, "$0", "%eax", "", "zero out EAX without changing ZF"));
    linked_list_insert_tail(list,
                            new_asm_node(e->lineno,
                                         setne,
                                         "%al",
                                         "",
                                         "",
                                         "set AL register (the lower byte of EAX) to 1 iff e1 | e2 != 0"));

    return;
}

void code_and_expression(linked_list *list, expression *e) {
    eval_expression(list, e->val.ops.left);
    linked_list_insert_tail(list, new_asm_node(e->lineno, push, "%eax", "", "", "save value of e1 on the stack"));
    eval_expression(list, e->val.ops.right);
    linked_list_insert_tail(list, new_asm_node(e->lineno, pop, "%ecx", "", "", "pop e1 from the stack into ecx"));
    //"; Step 1: SET CL = 1 iff e1 != 0")); TODO Debug
    linked_list_insert_tail(list, new_asm_node(e->lineno, cmpl, "$0", "%ecx", "", "compare e1 to 0"));
    linked_list_insert_tail(list, new_asm_node(e->lineno, setne, "%cl", "", "", "set CL to 1 iff e1 != 0"));
    //; Step 2: SET AL = 1 iff e2 != 0")); //TODO DEBUG
    linked_list_insert_tail(list, new_asm_node(e->lineno, cmpl, "$0", "%eax", "", "compare e2 to 0"));
    linked_list_insert_tail(list,
                            new_asm_node(e->lineno, movl, "$0", "%eax", "", "zero EAX register before storing result"));
    linked_list_insert_tail(list, new_asm_node(e->lineno, setne, "%al", "", "", "set AL to 1 iff e2 != 0"));
    //; Step 3: compute al & cl")); //TODO Debug
    linked_list_insert_tail(list, new_asm_node(e->lineno, andb, "%cl", "%al", "", "store AL & CL in AL"));
    return;
}

//todo
void code_general_expression(linked_list *list, expression *e, ASM_kind kind) {
    eval_expression(list, e->val.ops.left);

    linked_list_insert_tail(list, new_asm_node(e->lineno, push, "%eax", "", "", "save value of e1 on the stack"));
    eval_expression(list, e->val.ops.right);
    linked_list_insert_tail(list, new_asm_node(e->lineno, pop, "%ecx", "", "", "pop e1 from the stack into ecx"));
    linked_list_insert_tail(list, new_asm_node(e->lineno, cmp, "ecx", "eax", "", "values are now at EAX and ECX"));
    linked_list_insert_tail(list, new_asm_node(e->lineno, cmp, "ecx", "eax", "", "values are now at EAX and ECX"));
    linked_list_insert_tail(list,
                            new_asm_node(e->lineno, kind, "lable", "eax", "", "Jump to flag if expression is true"));
    return;
}
/*

void code_GEQ_statement(linked_list *list, expression *e) {
    eval_expression(e->val.ops.left);
    linked_list_insert_tail(("push  %eax            ;save value of e1 on the stack"));
    eval_expression(e->val.ops.right);
    linked_list_insert_tail(("pop   %ecx            ;pop e1 from the stack into ECX"));
    linked_list_insert_tail(("cmp ecx, eax          ;values are now at EAX and ECX"));
    linked_list_insert_tail(("jge FLAG               ;Jump to flag if expression is false"));
    return;
}

void code_LEQ_statement(linked_list *list, expression *e) {
    eval_expression(e->val.ops.left);
    linked_list_insert_tail("push  %eax            ;save value of e1 on the stack");
    eval_expression(e->val.ops.right);
    linked_list_insert_tail("pop   %ecx            ;pop e1 from the stack into ECX");
    linked_list_insert_tail("cmp ecx, eax          ;values are now at EAX and ECX");
    linked_list_insert_tail(("jle FLAG               ;Jump to flag if expression is false"));
    return;
}

void code_LT_statement(linked_list *list, expression *e) {
    eval_expression(e->val.ops.left);
    linked_list_insert_tail("push  %eax            ;save value of e1 on the stack");
    eval_expression(e->val.ops.right);
    linked_list_insert_tail("pop   %ecx            ;pop e1 from the stack into ECX");
    linked_list_insert_tail("cmp ecx, eax          ;values are now at EAX and ECX");
    linked_list_insert_tail(("jl FLAG               ;Jump to flag if expression is false"));
    return;
}

void code_GT_statement(linked_list *list, expression *e) {
    eval_expression(e->val.ops.left);
    linked_list_insert_tail(("push  %eax            ;save value of e1 on the stack"));
    eval_expression(e->val.ops.right);
    linked_list_insert_tail(("pop   %ecx            ;pop e1 from the stack into ECX"));
    linked_list_insert_tail(("cmp ecx, eax          ;values are now at EAX and ECX"));
    linked_list_insert_tail(("jg FLAG               ;Jump to flag if expression is false"));
    return;
}

void code_NEQ_statement(linked_list *list, expression *e) {
    eval_expression(e->val.ops.left);
    linked_list_insert_tail(("push  %eax            ;save value of e1 on the stack"));
    eval_expression(e->val.ops.right);
    linked_list_insert_tail(("pop   %ecx            ;pop e1 from the stack into ECX"));
    linked_list_insert_tail(("cmp ecx, eax          ;values are now at EAX and ECX"));
    linked_list_insert_tail(("jne FLAG               ;Jump to flag if expression is false"));
    return;
}

void code_EQ_statement(linked_list *list, expression *e) {
    eval_expression(e->val.ops.left);
    linked_list_insert_tail(("push  %eax            ;save value of e1 on the stack"));
    eval_expression(e->val.ops.right);
    linked_list_insert_tail("pop   %ecx            ;pop e1 from the stack into ECX");
    linked_list_insert_tail("cmp ecx, eax          ;values are now at EAX and ECX");
    linked_list_insert_tail("je FLAG               ;Jump to flag if expression is false");
    return;
}
*/
//TODO Lots

void code_plus_expression(linked_list *list, expression *e) {

    eval_expression(list, e->val.ops.left);

    linked_list_insert_tail(list, new_asm_node(e->lineno, push, "eax", "", "", "save value of e1 on the stack"));
    eval_expression(list, e->val.ops.right);
    linked_list_insert_tail(list, new_asm_node(e->lineno, pop, "ecx", "", "", "pop e1 from the stack into ecx"));
    linked_list_insert_tail(list, new_asm_node(e->lineno, add, "ecx", "eax", "", "add ECX and EAX are store result at EAX"));

}

void code_sub_expression(linked_list *list, expression *e) {

    eval_expression(list, e->val.ops.left);

    linked_list_insert_tail(list, new_asm_node(e->lineno, push, "eax", "", "", "save value of e1 on the stack"));
    eval_expression(list, e->val.ops.right);
    linked_list_insert_tail(list, new_asm_node(e->lineno, pop, "ecx", "", "", "pop e1 from the stack into ecx"));
    linked_list_insert_tail(list, new_asm_node(e->lineno, sub, "ecx", "eax", "", "add ECX and EAX are store result at EAX"));

}


void code_div_expression(linked_list *list, expression *e) {

    eval_expression(list, e->val.ops.left);

    linked_list_insert_tail(list, new_asm_node(e->lineno, push, "eax", "", "", "save value of e1 on the stack"));
    eval_expression(list, e->val.ops.right);
    linked_list_insert_tail(list, new_asm_node(e->lineno, pop, "ecx", "", "", "pop e1 from the stack into ecx"));
    //linked_list_insert_tail(list, new_asm_node(e->lineno, div, "ecx", "eax", "", "add ECX and EAX are store result at EAX"));

}

void code_mul_expression(linked_list *list, expression *e) {

    eval_expression(list, e->val.ops.left);

    linked_list_insert_tail(list, new_asm_node(e->lineno, push, "eax", "", "", "save value of e1 on the stack"));
    eval_expression(list, e->val.ops.right);
    linked_list_insert_tail(list, new_asm_node(e->lineno, pop, "ecx", "", "", "pop e1 from the stack into ecx"));
    linked_list_insert_tail(list, new_asm_node(e->lineno, mul, "ecx", "eax", "", "add ECX and EAX are store result at EAX"));

}


void code_term_expression(linked_list *list, expression *e) {
    e->val;

}


void code_term_expression(linked_list *pList, expression *pExpression);
void eval_expression(linked_list *list, expression *e) {
    switch (e->kind) {
    case exp_PLUS:
        code_plus_expression(list, e);
        break;
    case exp_MIN:
        code_sub_expression(list, e);
        break;
    case exp_MULT:
        code_mul_expression(list, e);
        break;
    case exp_DIV:
        code_div_expression(list,e);
        break;
    case exp_EQ:
        code_general_expression(list, e, je);
        break;
    case exp_NEQ:
        code_general_expression(list, e, jne);
        break;
    case exp_GT:
        code_general_expression(list, e, jg);
        break;
    case exp_LT:
        code_general_expression(list, e, jl);
        break;
    case exp_GEQ:
        code_general_expression(list, e, jge);
        break;
    case exp_LEQ:
        code_general_expression(list, e, jle);
        break;
    case exp_AND:
        code_and_expression(list, e);
        break;
    case exp_OR:
        code_or_expression(list, e);
        break;
    case exp_TERM:
        eval_term(list, e->val.term);
        break;
    }
    return;
}

//TODO Lots
void eval_term(linked_list *list, term *t) {

    switch (t->kind) {
    case term_VAR:break;
    case term_LIST:break;
    case term_PAR:break;
    case term_NOT:break;
    case term_ABS:break;
    case term_NUM:
        linked_list_insert_tail(list, new_asm_node(t->lineno, mov, "BYTE","1", "eax",  "setting eax to True"));
        break;
    case term_TRUE:
        linked_list_insert_tail(list, new_asm_node(t->lineno, mov, "BYTE","1", "eax",  "setting eax to True"));
        break;
    case term_FALSE:
        linked_list_insert_tail(list, new_asm_node(t->lineno, mov, "BYTE","0", "eax",  "setting eax to False"));
        break;
    case term_NULL:
        linked_list_insert_tail(list, new_asm_node(t->lineno, mov, "\0", "eax", "", "setting eax to null"));
        break;
    }
    return;
}


//TODO Lots
void eval_statement(linked_list *list, statement *s) {

    switch (s->kind) {
    case statement_WRITE:
        break;
    case statement_ALLOCATE:
        break;
    case statement_ALLOCATE_LENGTH:
        break;
    case statement_ASSIGNMENT:
        break;
    case statement_IF:
        break;
    case statement_IF_ELSE:
        break;
    case statement_WHILE:
        break;
    case statement_LIST:
        break;
    case statement_RETURN:
        break;
    }
    return;
}

//TODO this is just a sample piece of test code.
linked_list *buildcode(statement_list input) {
    linked_list *list = NULL;
    //TODO this needs to be changed.
    eval_statement(list, input.statement);

    //this probably isnt needed but for now it's returned, its a global variable so can be called from anywhere.
    return list;
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
	b = generate_body(body);

	asm_insert(&head, &tail, &b);
	printf("Done generating program\n");

	return head;
   
}

a_asm *generate_body(body *body){
	struct a_asm *sl;
	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;

	printf("In body, generating s_list\n");
	sl = generate_slist(body->s_list);
	asm_insert(&head, &tail, &sl);
	printf("In body, done generating s_list\n");

	return head;
	
}

a_asm *generate_function(function *func){
	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;
    

	return head;
	
}

a_asm *generate_head(head *h){
	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;
    

	return head;
}

a_asm *generate_tail(tail *t){
	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;
    

	return head;
}

a_asm *generate_type(type *type){
	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;
    

	return head;
}

a_asm *generate_pdl(par_decl_list *pdl){
	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;
    

	return head;
}

a_asm *generate_vdl(var_decl_list *vdl){
	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;
    

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
	head = NULL;
	tail = NULL;
    

	return head;
}

a_asm *generate_decl(declaration *decl){
	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;
    

	return head;
}

a_asm *generate_slist(statement_list *slist){
	struct a_asm *stmt;
	struct a_asm *sl;

	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;


	printf("In s_list, generating statement\n");
	stmt = generate_stmt(slist->statement);
	
	asm_insert(&head, &tail, &stmt);

	printf("In s_list, done generating statement\n");

	if (slist->kind !=NULL){
		printf("In s_list, generating s_list\n");
		sl = generate_slist(slist->list);
		asm_insert(&head, &tail, &sl);
		printf("In s_list, done generating s_list\n");
	}

	// Could possibly be a struct = {head, tail}, so we didn't have to loop through and find the tail later.
	return head;
    
}

a_asm *generate_stmt(statement *stmt){
	struct a_asm *head;
    struct a_asm *tail;    printf("Creating 2 op\n");

	head = NULL;
	tail = NULL;
	struct a_asm *expr;

	struct asm_op *wrt;


	switch (stmt->kind){

		case (statement_WRITE):

			printf("In statement, generating expression\n");
			expr = generate_exp(stmt->val.wrt);
			printf("In statment, done generating expressions\n");
			asm_insert(&head, &tail, &expr);
			wrt = tail->val.two_op.op2;
			printf("Got expression to write\n");
			//Should probably be some more pushing before creating a print
			add_1_ins(&head, &tail, PUSH, reg_RAX, "Saving value of RAX before printf call");

			add_2_ins(&head, &tail, MOVQ, wrt_INT, reg_RDI, "First argument for printf");
			add_2_ins(&head, &tail, MOVQ, wrt, reg_RSI, "Second argument for printf");
			add_2_ins(&head, &tail, MOVQ, make_op_const(0), reg_RAX, "No vector arguments");
			add_1_ins(&head, &tail, CALL, op_PRINTF, "Calling printf");

			add_1_ins(&head, &tail, POP, reg_RAX, "Restoring RAX");

			break;


		default:
			break;

	}
    
	return head;
}

a_asm *generate_exp(expression *exp){
	printf("Generating code for exp kind: %d\n", exp->kind);

    struct a_asm *left_exp;
    struct a_asm *right_exp;
    struct a_asm *single;
	struct asm_op *left_target;
	struct asm_op *right_target;

    struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;
    

    if (exp->kind == exp_TERM){

		printf("In expression, generating term\n");
        single = generate_term(exp->val.term);
		printf("Got term a_asm\n");
		asm_insert(&head, &tail, &single);
		printf("Inserted term in list\n");
		return head;
    }

	printf("In expression, generating left expression\n");
    left_exp = generate_exp(exp->val.ops.left);
    asm_insert(&head, &tail, &left_exp);
	left_target = tail->val.two_op.op2;

	printf("In expression, generating right expression\n");
	right_exp = generate_exp(exp->val.ops.right);
	asm_insert(&head, &tail, &right_exp);
	right_target = tail->val.two_op.op2;

	switch (exp->kind){

		case (exp_MULT):
			//IMUL uses the %rax register, so we must move one of the values to this register
			//Put the resulting value in a temporary
			add_2_ins(&head, &tail, MOVQ, left_target, reg_RAX, "Using RAX for multiplication");
			add_2_ins(&head, &tail, MOVQ, right_target, reg_RBX, "Using RBX for multiplication");
			add_1_ins(&head, &tail, IMUL, reg_RBX, "Multiplication using RAX and RBX");
			add_2_ins(&head, &tail, MOVQ, reg_RAX, make_op_temp, "Storing result here (temp)");
			break;

		case (exp_DIV):
			//IDIV uses the %rax register, so we must move one of the values to this register
			//Should be change to a multiplication when optimizing, since that is faster than division
			add_2_ins(&head, &tail, MOVQ, left_target, reg_RAX, "Using RAX for division");

			//Need to add a check to see if this value is 0 or not
			add_2_ins(&head, &tail, MOVQ, right_target, reg_RBX, "Using RBX for division");
			add_1_ins(&head, &tail, IDIV, reg_RBX, "Division using RAX and RBX");
			add_2_ins(&head, &tail, MOVQ, reg_RAX, make_op_temp, "Storing result here (temp)");
			break;

		case (exp_PLUS):
			add_2_ins(&head, &tail, ADDQ, left_target, right_target, "Addition");
			break;

		case (exp_MIN):
			add_2_ins(&head, &tail, SUBQ, left_target, right_target, "Subtraction");
			break;




	}

    return head;
    
}

a_asm *generate_term(term *term){
	struct a_asm *head;
    struct a_asm *tail;
	head = NULL;
	tail = NULL;

    switch (term->kind){

        case (term_NUM):

			printf("In term, generating num\n");
			add_2_ins(&head, &tail, MOVQ, make_op_const(term->val.num), make_op_temp(), "Moving constant to register");
			break;

		default:
			break;




    }
	printf("In term, returning head, head ins: %d\n", head->ins);

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
    

	return head;
}

// Insert a "linked list" into an existing list
void asm_insert(a_asm **head, a_asm **tail, a_asm **new){
	printf("Inserting in list\n");
    struct a_asm *temp;
	temp = NULL;
    
    if ((*new) != NULL){
		printf("Incoming list is not NULL\n");
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
	if (*tail == NULL){
		printf("Tail is NULL, when adding ins\n");
	}
	printf("Getting next...\n");
	get_next(head, tail);
	printf("Got next!\n");
	if (tail == NULL){
		printf("tail is still NULL\n");
	}
	
	(*tail)->ins = ins;
	(*tail)->val.two_op.op1 = op1;
	(*tail)->val.two_op.op2 = op2;
	(*tail)->comment = comment;
	(*tail)->ops = 2;
	(*tail)->next = NULL;
	printf("Tail ins: %d\n", (*tail)->ins);
	printf("Head ins: %d\n", (*head)->ins);

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



void get_next(a_asm **head1, a_asm **tail1){
	if (*tail1 == NULL){
		printf("Tail is null, creating new tail\n");
		*tail1 = NEW(a_asm);
		if (tail1 == NULL){
			printf("Tail is NULL after allocation\n");
		}
		*head1 = *tail1;
	} else {
		(*tail1)->next = NEW(a_asm);
		(*tail1)->next->prev = *tail1;
		*tail1 = (*tail1)->next;
	}

}

//Should no be necessary if we keep meta information on blocks
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