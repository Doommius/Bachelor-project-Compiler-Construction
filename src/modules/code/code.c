#include <string.h>
#include "tree.h"
#include "code.h"

char *code_or_expression(expression *e) {
    char output = eval_expression(e->val.ops.left);
    strcat(output, ("push  %eax           ;save value of e1 on the stack"));
    strcat(output, eval_expression(e->val.ops.left));
    strcat(output, ("pop   %ecx           ;pop e1 from the stack into ecx"));
    strcat(output, ("orl   %ecx, %eax     ;compute e1 | e2, set ZF"));
    strcat(output, ("movl  $0, %eax       ;zero out EAX without changing ZF"));
    strcat(output, ("setne %al            ;set AL register (the lower byte of EAX) to 1 iff e1 | e2 != 0"));
    return output;
}

char *code_and_expression(expression *e) {
    char output = eval_expression(e->val.ops.left);
    strcat(output, ("push  %eax            ;save value of e1 on the stack"));
    strcat(output, (eval_expression(e->val.ops.right)));
    strcat(output, ("pop   %ecx            ;pop e1 from the stack into ECX"));
    strcat(output, ("; Step 1: SET CL = 1 iff e1 != 0"));
    strcat(output, ("cmpl  $0, %ecx        ;compare e1 to 0"));
    strcat(output, ("setne %cl             ;set CL to 1 iff e1 != 0"));
    strcat(output, ("        ; Step 2: SET AL = 1 iff e2 != 0"));
    strcat(output, ("cmpl  $0, %eax        ;compare e2 to 0"));
    strcat(output, ("movl  $0, %eax        ;zero EAX register before storing result"));
    strcat(output, ("setne %al             ;set AL to 1 iff e2 != 0"));
    strcat(output, ("        ; Step 3: compute al & cl"));
    strcat(output, ("andb  %cl, %al        ;store AL & CL in AL"));
    return output;
}

char *code_General_statement(expression *e, char jumpif) {
    char output = eval_expression(e->val.ops.left);
    strcat(output, ("push  %eax            ;save value of e1 on the stack"));
    strcat(output, (eval_expression(e->val.ops.right)));
    strcat(output, ("pop   %ecx            ;pop e1 from the stack into ECX"));
    strcat(output, ("cmp ecx, eax          ;values are now at EAX and ECX"));
    strcat(output, ("jge INSERT jumpif here;Jump to flag if expression is false"));
    return output;
}


char *code_GEQ_statement(expression *e) {
    char output = eval_expression(e->val.ops.left);
    strcat(output, ("push  %eax            ;save value of e1 on the stack"));
    strcat(output, (eval_expression(e->val.ops.right)));
    strcat(output, ("pop   %ecx            ;pop e1 from the stack into ECX"));
    strcat(output, ("cmp ecx, eax          ;values are now at EAX and ECX"));
    strcat(output, ("jge FLAG               ;Jump to flag if expression is false"));
    return output;
}




char *code_LEQ_statement(expression *e) {
    char output = eval_expression(e->val.ops.left);
    strcat(output, ("push  %eax            ;save value of e1 on the stack"));
    strcat(output, (eval_expression(e->val.ops.right)));
    strcat(output, ("pop   %ecx            ;pop e1 from the stack into ECX"));
    strcat(output, ("cmp ecx, eax          ;values are now at EAX and ECX"));
    strcat(output, ("jle FLAG               ;Jump to flag if expression is false"));
    return output;
}

char *code_LT_statement(expression *e) {
    char output = eval_expression(e->val.ops.left);
    strcat(output, ("push  %eax            ;save value of e1 on the stack"));
    strcat(output, (eval_expression(e->val.ops.right)));
    strcat(output, ("pop   %ecx            ;pop e1 from the stack into ECX"));
    strcat(output, ("cmp ecx, eax          ;values are now at EAX and ECX"));
    strcat(output, ("jl FLAG               ;Jump to flag if expression is false"));
    return output;
}
char *code_GT_statement(expression *e) {
    char output = eval_expression(e->val.ops.left);
    strcat(output, ("push  %eax            ;save value of e1 on the stack"));
    strcat(output, (eval_expression(e->val.ops.right)));
    strcat(output, ("pop   %ecx            ;pop e1 from the stack into ECX"));
    strcat(output, ("cmp ecx, eax          ;values are now at EAX and ECX"));
    strcat(output, ("jg FLAG               ;Jump to flag if expression is false"));
    return output;
}

char *code_NEQ_statement(expression *e) {
    char output = eval_expression(e->val.ops.left);
    strcat(output, ("push  %eax            ;save value of e1 on the stack"));
    strcat(output, (eval_expression(e->val.ops.right)));
    strcat(output, ("pop   %ecx            ;pop e1 from the stack into ECX"));
    strcat(output, ("cmp ecx, eax          ;values are now at EAX and ECX"));
    strcat(output, ("jne FLAG               ;Jump to flag if expression is false"));
    return output;
}

char *code_EQ_statement(expression *e) {
    char output = eval_expression(e->val.ops.left);
    strcat(output, ("push  %eax            ;save value of e1 on the stack"));
    strcat(output, (eval_expression(e->val.ops.right)));
    strcat(output, ("pop   %ecx            ;pop e1 from the stack into ECX"));
    strcat(output, ("cmp ecx, eax          ;values are now at EAX and ECX"));
    strcat(output, ("je FLAG               ;Jump to flag if expression is false"));
    return output;
}





//TODO Lots
char *eval_expression(expression *e) {
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
        return code_and_expression(e);
        break;
    case exp_OR:
        return code_or_expression(e);
        break;
    case exp_TERM:
        break;
    }
    return " ";
}

char *eval_statement(statement *s) {

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
    }
    return " ";
}
