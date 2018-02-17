#include "memory.h"
#include "tree.h"

extern int lineno;

type *make_type_id(char *id) {
    type *t;
    t = NEW(type);
    t->lineno = lineno;
    t->kind = type_ID;
    t->val.id = id;
    return t;
}

expression *make_EXP(EXP_kind kind, expression *left, expression *right){
    expression *e;
    e = NEW(expression);
    e->lineno = lineno;
    e->kind = kind;
    e->val.ops.left = left;
    e->val.ops.right = right;
    return e;
}

expression *make_EXP_term(term *term){
    expression *e;
    e = NEW(expression);
    e->lineno = lineno;
    e->kind = exp_TERM;
    e->val.term = term;
    return e;
}

term *make_Term_num(int intconst){
    term *t;
    t = NEW(term);
    t->lineno = lineno;
    t->kind = term_NUM;
    t->val.num = intconst;
    return t;
}

/*
EXP *makeEXPArithmeticstructure(EXP *left, EXP *right, kindArithmetic kind) {
    EXP *e;
    e = NEW(EXP);
    e->lineno = lineno;
    e->kind = kind;
    e->data.val.left = left;
    e->data.val.right = right;
    return e;

}
*/


