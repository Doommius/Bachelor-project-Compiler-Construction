#include <stdio.h>
#include "pretty.h"
#include "tree.h"

void prettyEXP(expression *e) {
    switch (e->kind) {
        /*
        case idK:
            printf("%s", e->data.idE);
            break;
        case intconstK:
            printf("%i", e->data.intconstE);
            break;
        case timesK:
            printf("(");
            prettyEXP(e->data.val.left);
            printf("*");
            prettyEXP(e->data.val.right);
            printf(")");
            break;
        case divK:
            printf("(");
            prettyEXP(e->data.val.left);
            printf("/");
            prettyEXP(e->data.val.right);
            printf(")");
            break;
           */
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
        case exp_TERM:
            prettyTerm(e->val.term);
            break;
            /*
        case modK:
            printf("(");
            prettyEXP(e->data.val.left);
            printf("%%");
            prettyEXP(e->data.val.right);
            printf(")");
            break;
             */
    }
}

void prettyTerm(term *t){
    switch (t->kind){
        case term_NUM:
            printf("%i", t->val.num);

            break;
    }
}