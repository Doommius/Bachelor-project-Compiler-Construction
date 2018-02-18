#include "memory.h"
#include "tree.h"
#include <stdio.h>

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

term *make_Term_par(expression *expression){
    term *t;
    t = NEW(term);
    t->lineno = lineno;
    t->kind = term_PAR;
    t->val.expression = expression;
    return t;
}

term *make_Term_not(term *term){
    struct term *t;
    t = NEW(struct term);
    t->lineno = lineno;
    t->kind = term_NOT;
    t->val.term_not = term;
    return t;
}

term *make_Term_abs(expression *expression){
    term *t;
    t = NEW(term);
    t->lineno = lineno;
    t->kind = term_ABS;
    t->val.expression = expression;
    return t;
}

term *make_Term_boolean(int bool){
    term *t;
    t = NEW(term);
    t->lineno = lineno;
    if (bool == 1){
        t->kind = term_TRUE;
        return t;
    }
    t->kind = term_FALSE;
    return t;
}

term *make_Term_null(){
    term *t;
    t = NEW(term);
    t->lineno = lineno;
    t->kind = term_NULL;
    return t;
}

term *make_Term_variable(variable *var){
    term *t;
    t = NEW(term);
    t->lineno = lineno;
    t->kind = term_VAR;
    t->val.variable = var;
    return t;
}

term *make_Term_list(char *id, act_list *list){
    term *t;
    t = NEW(term);
    t->lineno = lineno;
    t->kind = term_LIST;
    t->val.list.id = id;
    t->val.list.list = list;
    return t;
}

act_list *make_Act_list(exp_list *list){
    act_list *al;
    al = NEW(act_list);
    al->lineno = lineno;
    al->kind = al_LIST;
    al->list = list;
    return al;
}
act_list *make_Act_empty(){
    act_list *al;
    al = NEW(act_list);
    al->lineno = lineno;
    al->kind = al_EMPTY;
    al->list = NULL;
    return al;
}

exp_list *make_ExpL_exp(expression *expression){
    exp_list *el;
    el = NEW(exp_list);
    el->lineno = lineno;
    el->kind = el_EXP;
    el->expression = expression;
    return el;

}

exp_list *make_ExpL_list(expression *expression, exp_list *list){
    exp_list *el;
    el = NEW(exp_list);
    el->lineno = lineno;
    el->kind = el_LIST;
    el->expression = expression;
    el->list = list;
    return el;
}

variable *make_Var_id(char *id){
    variable *v;
    v = NEW(variable);
    v->lineno = lineno;
    v->kind = var_ID;
    v->id = id;
    return v;
}

variable *make_Var_exp(variable *var, expression *expression){
    variable *v;
    v = NEW(variable);
    v->lineno = lineno;
    v->kind = var_EXP;
    v->val.exp.var = var;
    v->val.exp.exp = expression;
    return v;
}


variable *make_Var_varid(variable *var, char *id){
    variable *v;
    v = NEW(variable);
    v->lineno = lineno;
    v->kind = var_VID;
    v->val.varid.var = var;
    v->val.varid.id = id;
    return v;
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


