#include <stdio.h>
#include "pretty.h"
#include "tree.h"

void prettyVar(variable *v){
    switch (v->kind){

        case var_ID:
            printf("%s", v->id);
            break;

        case var_EXP:
            prettyVar(v->val.exp.var);
            printf("[");
            prettyEXP(v->val.exp.exp);
            printf("]");
            break;

        case var_VID:
            prettyVar(v->val.varid.var);
            printf(".%s", v->val.varid.id);
            break;

    }
}

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

        case term_VAR:
            prettyVar(t->val.variable);
            break;

        case term_LIST:
            printf("%s (", t->val.list.id);
            prettyAL(t->val.list.list);
            printf(")");
            break;

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

void prettyAL(act_list *al){
    switch (al->kind){

        case al_LIST:
            prettyEL(al->list);
            break;

        case al_EMPTY:
            break;

    }
}


void prettyEL(exp_list *el){
    switch (el->kind){

        case el_EXP:
            prettyEXP(el->expression);
            break;

        case el_LIST:
            prettyEXP(el->expression);
            printf(", ");
            prettyEL(el->list);
            break;
    }

}