#include <string.h>
#include "linked_list.h"
#include "tree.h"
#include "code.h"

void code_or_expression(linked_list *list, expression *e) {
    eval_expression(list, e->val.ops.left);
    linked_list_insert_tail(list, new_asm_node(e->lineno, push, "%eax", "", "", "save value of e1 on the stack"));
    eval_expression(list, e->val.ops.left);
    linked_list_insert_tail(list, new_asm_node(e->lineno, pop, "%ecx", "", "", "pop e1 from the stack into ecx"));
    linked_list_insert_tail(list, new_asm_node(e->lineno, orl, "%ecx", "%eax", "", "compute e1 | e2, set ZF"));
    linked_list_insert_tail(list, new_asm_node(e->lineno, movl, "$0", "%eax", "", "zero out EAX without changing ZF"));
    linked_list_insert_tail(list, new_asm_node(e->lineno, setne, "%al", "", "", "set AL register (the lower byte of EAX) to 1 iff e1 | e2 != 0"));

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
    linked_list_insert_tail(list, new_asm_node(e->lineno, movl, "$0", "%eax", "", "zero EAX register before storing result"));
    linked_list_insert_tail(list, new_asm_node(e->lineno, setne, "%al", "", "", "set AL to 1 iff e2 != 0"));
    //; Step 3: compute al & cl")); //TODO Debug
    linked_list_insert_tail(list, new_asm_node(e->lineno, andb, "%cl", "%al", "", "store AL & CL in AL"));
    return;
}

//todo
void code_general_statement(linked_list *list, expression *e, ASM_kind kind) {
    eval_expression(list,e->val.ops.left);

    linked_list_insert_tail(list, new_asm_node(e->lineno, push, "%eax", "", "", "save value of e1 on the stack"));
    eval_expression(list,e->val.ops.right);
    linked_list_insert_tail(list, new_asm_node(e->lineno, pop, "%ecx", "", "", "pop e1 from the stack into ecx"));
    linked_list_insert_tail(list, new_asm_node(e->lineno, cmp, "ecx", "eax", "", "values are now at EAX and ECX"));
    linked_list_insert_tail(list, new_asm_node(e->lineno, cmp, "ecx", "eax", "", "values are now at EAX and ECX"));
    linked_list_insert_tail(list, new_asm_node(e->lineno, kind, "lable", "eax", "", "Jump to flag if expression is true"));
    return;
}
//
//void code_GEQ_statement(linked_list *list, expression *e) {
//    eval_expression(e->val.ops.left);
//    linked_list_insert_tail(("push  %eax            ;save value of e1 on the stack"));
//    eval_expression(e->val.ops.right);
//    linked_list_insert_tail(("pop   %ecx            ;pop e1 from the stack into ECX"));
//    linked_list_insert_tail(("cmp ecx, eax          ;values are now at EAX and ECX"));
//    linked_list_insert_tail(("jge FLAG               ;Jump to flag if expression is false"));
//    return;
//}
//
//void code_LEQ_statement(linked_list *list, expression *e) {
//    eval_expression(e->val.ops.left);
//    linked_list_insert_tail("push  %eax            ;save value of e1 on the stack");
//    eval_expression(e->val.ops.right);
//    linked_list_insert_tail("pop   %ecx            ;pop e1 from the stack into ECX");
//    linked_list_insert_tail("cmp ecx, eax          ;values are now at EAX and ECX");
//    linked_list_insert_tail(("jle FLAG               ;Jump to flag if expression is false"));
//    return;
//}
//
//void code_LT_statement(linked_list *list, expression *e) {
//    eval_expression(e->val.ops.left);
//    linked_list_insert_tail("push  %eax            ;save value of e1 on the stack");
//    eval_expression(e->val.ops.right);
//    linked_list_insert_tail("pop   %ecx            ;pop e1 from the stack into ECX");
//    linked_list_insert_tail("cmp ecx, eax          ;values are now at EAX and ECX");
//    linked_list_insert_tail(("jl FLAG               ;Jump to flag if expression is false"));
//    return;
//}
//
//void code_GT_statement(linked_list *list, expression *e) {
//    eval_expression(e->val.ops.left);
//    linked_list_insert_tail(("push  %eax            ;save value of e1 on the stack"));
//    eval_expression(e->val.ops.right);
//    linked_list_insert_tail(("pop   %ecx            ;pop e1 from the stack into ECX"));
//    linked_list_insert_tail(("cmp ecx, eax          ;values are now at EAX and ECX"));
//    linked_list_insert_tail(("jg FLAG               ;Jump to flag if expression is false"));
//    return;
//}
//
//void code_NEQ_statement(linked_list *list, expression *e) {
//    eval_expression(e->val.ops.left);
//    linked_list_insert_tail(("push  %eax            ;save value of e1 on the stack"));
//    eval_expression(e->val.ops.right);
//    linked_list_insert_tail(("pop   %ecx            ;pop e1 from the stack into ECX"));
//    linked_list_insert_tail(("cmp ecx, eax          ;values are now at EAX and ECX"));
//    linked_list_insert_tail(("jne FLAG               ;Jump to flag if expression is false"));
//    return;
//}
//
//void code_EQ_statement(linked_list *list, expression *e) {
//    eval_expression(e->val.ops.left);
//    linked_list_insert_tail(("push  %eax            ;save value of e1 on the stack"));
//    eval_expression(e->val.ops.right);
//    linked_list_insert_tail("pop   %ecx            ;pop e1 from the stack into ECX");
//    linked_list_insert_tail("cmp ecx, eax          ;values are now at EAX and ECX");
//    linked_list_insert_tail("je FLAG               ;Jump to flag if expression is false");
//    return;
//}

//TODO Lots
void eval_expression(linked_list *list, expression *e) {
    switch (e->kind) {
    case exp_PLUS:
        break;
    case exp_MIN:
        break;
    case exp_MULT:
        break;
    case exp_DIV:
        break;
    case exp_EQ:
        code_general_statement(list,e, je);
        break;
    case exp_NEQ:
        code_general_statement(list,e, jne);
        break;
    case exp_GT:
        code_general_statement(list,e, jg);
        break;
    case exp_LT:
        code_general_statement(list,e, jl);
        break;
    case exp_GEQ:
        code_general_statement(list,e, jge);
        break;
    case exp_LEQ:
        code_general_statement(list,e, jle);
        break;
    case exp_AND:
        code_and_expression(list, e);
        break;
    case exp_OR:
        code_or_expression(list, e);
        break;
    case exp_TERM:
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
