#include <string.h>
#include "linked_list.h"
#include "tree.h"
#include "code.h"

void code_or_expression(expression *e) {
    eval_expression(e->val.ops.left);
    linked_list_insert_tail(("push  %eax           ;save value of e1 on the stack"));
    eval_expression(e->val.ops.left);
    linked_list_insert_tail(("pop   %ecx           ;pop e1 from the stack into ecx"));
    linked_list_insert_tail(("orl   %ecx, %eax     ;compute e1 | e2, set ZF"));
    linked_list_insert_tail(("movl  $0, %eax       ;zero out EAX without changing ZF"));
    linked_list_insert_tail(("setne %al            ;set AL register (the lower byte of EAX) to 1 iff e1 | e2 != 0"));
    return;
}

void code_and_expression(expression *e) {
    eval_expression(e->val.ops.left);
    linked_list_insert_tail(("push  %eax            ;save value of e1 on the stack"));
    eval_expression(e->val.ops.right);
    linked_list_insert_tail(("pop   %ecx            ;pop e1 from the stack into ECX"));
    linked_list_insert_tail(("; Step 1: SET CL = 1 iff e1 != 0"));
    linked_list_insert_tail(("cmpl  $0, %ecx        ;compare e1 to 0"));
    linked_list_insert_tail(("setne %cl             ;set CL to 1 iff e1 != 0"));
    linked_list_insert_tail(("        ; Step 2: SET AL = 1 iff e2 != 0"));
    linked_list_insert_tail(("cmpl  $0, %eax        ;compare e2 to 0"));
    linked_list_insert_tail(("movl  $0, %eax        ;zero EAX register before storing result"));
    linked_list_insert_tail(("setne %al             ;set AL to 1 iff e2 != 0"));
    linked_list_insert_tail(("        ; Step 3: compute al & cl"));
    linked_list_insert_tail(("andb  %cl, %al        ;store AL & CL in AL"));
    return;
}

//todo
void code_general_statement(expression *e, char jumpif) {
    eval_expression(e->val.ops.left);
    linked_list_insert_tail(("push  %eax            ;save value of e1 on the stack"));
    eval_expression(e->val.ops.right);
    linked_list_insert_tail("pop   %ecx            ;pop e1 from the stack into ECX");
    linked_list_insert_tail(("cmp ecx, eax          ;values are now at EAX and ECX"));
    linked_list_insert_tail(("jge INSERT jumpif here;Jump to flag if expression is false"));
    return;
}

void code_GEQ_statement(expression *e) {
    eval_expression(e->val.ops.left);
    linked_list_insert_tail(("push  %eax            ;save value of e1 on the stack"));
    eval_expression(e->val.ops.right);
    linked_list_insert_tail(("pop   %ecx            ;pop e1 from the stack into ECX"));
    linked_list_insert_tail(("cmp ecx, eax          ;values are now at EAX and ECX"));
    linked_list_insert_tail(("jge FLAG               ;Jump to flag if expression is false"));
    return;
}

void code_LEQ_statement(expression *e) {
    eval_expression(e->val.ops.left);
    linked_list_insert_tail("push  %eax            ;save value of e1 on the stack");
    eval_expression(e->val.ops.right);
    linked_list_insert_tail("pop   %ecx            ;pop e1 from the stack into ECX");
    linked_list_insert_tail("cmp ecx, eax          ;values are now at EAX and ECX");
    linked_list_insert_tail(("jle FLAG               ;Jump to flag if expression is false"));
    return;
}

void code_LT_statement(expression *e) {
    eval_expression(e->val.ops.left);
    linked_list_insert_tail("push  %eax            ;save value of e1 on the stack");
    eval_expression(e->val.ops.right);
    linked_list_insert_tail("pop   %ecx            ;pop e1 from the stack into ECX");
    linked_list_insert_tail("cmp ecx, eax          ;values are now at EAX and ECX");
    linked_list_insert_tail(("jl FLAG               ;Jump to flag if expression is false"));
    return;
}

void code_GT_statement(expression *e) {
    eval_expression(e->val.ops.left);
    linked_list_insert_tail(("push  %eax            ;save value of e1 on the stack"));
    eval_expression(e->val.ops.right);
    linked_list_insert_tail(("pop   %ecx            ;pop e1 from the stack into ECX"));
    linked_list_insert_tail(("cmp ecx, eax          ;values are now at EAX and ECX"));
    linked_list_insert_tail(("jg FLAG               ;Jump to flag if expression is false"));
    return;
}

void code_NEQ_statement(expression *e) {
    eval_expression(e->val.ops.left);
    linked_list_insert_tail(("push  %eax            ;save value of e1 on the stack"));
    eval_expression(e->val.ops.right);
    linked_list_insert_tail(("pop   %ecx            ;pop e1 from the stack into ECX"));
    linked_list_insert_tail(("cmp ecx, eax          ;values are now at EAX and ECX"));
    linked_list_insert_tail(("jne FLAG               ;Jump to flag if expression is false"));
    return;
}

void code_EQ_statement(expression *e) {
    eval_expression(e->val.ops.left);
    linked_list_insert_tail(("push  %eax            ;save value of e1 on the stack"));
    eval_expression(e->val.ops.right);
    linked_list_insert_tail("pop   %ecx            ;pop e1 from the stack into ECX");
    linked_list_insert_tail("cmp ecx, eax          ;values are now at EAX and ECX");
    linked_list_insert_tail("je FLAG               ;Jump to flag if expression is false");
    return;
}

//TODO Lots
void eval_expression(expression *e) {
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
        code_EQ_statement(e);
        break;
    case exp_NEQ:
        code_NEQ_statement(e);
        break;
    case exp_GT:
        code_GT_statement(e);
        break;
    case exp_LT:
        code_LT_statement(e);
        break;
    case exp_GEQ:
        code_GEQ_statement(e);
        break;
    case exp_LEQ:
        code_LEQ_statement(e);
        break;
    case exp_AND:
        code_and_expression(e);
        break;
    case exp_OR:
        code_or_expression(e);
        break;
    case exp_TERM:
        break;
    }
    return;
}

//TODO Lots
void eval_statement(statement *s) {

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
    linked_list_head = NULL;
    //TODO this needs to be changed.
    eval_statement(input.statement);

    //this probably isnt needed but for now it's returned, its a global variable so can be called from anywhere.
    return linked_list_head;
}
