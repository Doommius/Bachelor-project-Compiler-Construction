#include <stdio.h>
#include "pretty.h"
#include "tree.h"

void prettyEXP(expression *e) {
    switch (e->kind) {

        case exp_MULT:
            printf("(");
            prettyEXP(e->val.ops.left);
            printf("*");
            prettyEXP(e->val.ops.right);
            printf(")");
            break;

        case exp_DIV:
            printf("(");
            prettyEXP(e->val.ops.left);
            printf("/");
            prettyEXP(e->val.ops.right);
            printf(")");
            break;

        case exp_PLUS:
            printf("(");
            prettyEXP(e->val.ops.left);
            printf("+");
            prettyEXP(e->val.ops.right);
            printf(")");
            break;

        case exp_MIN:
            printf("(");
            prettyEXP(e->val.ops.left);
            printf("-");
            prettyEXP(e->val.ops.right);
            printf(")");
            break;

        case exp_EQ:
            printf("(");
            prettyEXP(e->val.ops.left);
            printf("==");
            prettyEXP(e->val.ops.right);
            printf(")");
            break;

        case exp_NEQ:
            printf("(");
            prettyEXP(e->val.ops.left);
            printf("!=");
            prettyEXP(e->val.ops.right);
            printf(")");
            break;

        case exp_GT:
            printf("(");
            prettyEXP(e->val.ops.left);
            printf(">");
            prettyEXP(e->val.ops.right);
            printf(")");
            break;

        case exp_LT:
            printf("(");
            prettyEXP(e->val.ops.left);
            printf("<");
            prettyEXP(e->val.ops.right);
            printf(")");
            break;

        case exp_GEQ:
            printf("(");
            prettyEXP(e->val.ops.left);
            printf(">=");
            prettyEXP(e->val.ops.right);
            printf(")");
            break;

        case exp_LEQ:
            printf("(");
            prettyEXP(e->val.ops.left);
            printf("<=");
            prettyEXP(e->val.ops.right);
            printf(")");
            break;

        case exp_AND:
            printf("(");
            prettyEXP(e->val.ops.left);
            printf("&&");
            prettyEXP(e->val.ops.right);
            printf(")");
            break;

        case exp_OR:
            printf("(");
            prettyEXP(e->val.ops.left);
            printf("||");
            prettyEXP(e->val.ops.right);
            printf(")");
            break;

        case exp_TERM:
            prettyTerm(e->val.term);
            break;

    }
}

void prettyTerm(term *t){
    switch (t->kind){
        case term_PAR:
            printf("(");
            prettyEXP(t->val.expression);
            printf(")");
            break;

        case term_NOT:
            printf("!");
            prettyTerm(t->val.term_not);
            break;

        case term_ABS:
            printf("|");
            prettyEXP(t->val.expression);
            printf("|");
            break;

        case term_TRUE:
            printf("true");
            break;

        case term_FALSE:
            printf("false");
            break;

        case term_NULL:
            printf("null");
            break;

        case term_NUM:
            printf("%i", t->val.num);
            break;
    }
}