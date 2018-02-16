#include <stdio.h>
#include "pretty.h"
#include "tree.h"

void prettyEXP(EXP *e) {
    switch (e->kind) {
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
        case plusK:
            printf("(");
            prettyEXP(e->data.val.left);
            printf("+");
            prettyEXP(e->data.val.right);
            printf(")");
            break;
        case minusK:
            printf("(");
            prettyEXP(e->data.val.left);
            printf("-");
            prettyEXP(e->data.val.right);
            printf(")");
            break;
        case modK:
            printf("(");
            prettyEXP(e->data.val.left);
            printf("%%");
            prettyEXP(e->data.val.right);
            printf(")");
            break;
    }
}
