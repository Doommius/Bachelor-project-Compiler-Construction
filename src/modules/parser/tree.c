/**
 * @brief 
 * 
 * @file tree.c
 * @author Morten Jæger, Mark Jervelund & Troels Blicher Petersen
 * @date 2018-03-09
 */
#include "memory.h"
#include "tree.h"
#include <stdio.h>

extern int lineno;

function *make_Func(head *h, body *b, tail *t) {
    function *f;
    f = NEW(function);
    f->lineno = lineno;
    f->head = h;
    f->body = b;
    f->tail = t;
    return f;
}

head *make_Head(char *id, par_decl_list *pdl, type *t) {
    head *h;
    h = NEW(head);
    h->lineno = lineno;
    h->id = id;
    h->list = pdl;
    h->type = t;
    return h;
}

tail *make_Tail(char *id) {
    tail *t;
    t = NEW(tail);
    t->lineno = lineno;
    t->id = id;
    return t;
}

type *make_Type_id(char *id) {
    type *t;
    t = NEW(type);
    t->lineno = lineno;
    t->kind = type_ID;
    t->val.id = id;
    return t;
}

type *make_Type_int() {
    type *t;
    t = NEW(type);
    t->lineno = lineno;
    t->kind = type_INT;
    return t;
}

type *make_Type_bool() {
    type *t;
    t = NEW(type);
    t->lineno = lineno;
    t->kind = type_BOOl;
    return t;
}

type *make_Type_array(type *t1) {
    type *t;
    t = NEW(type);
    t->lineno = lineno;
    t->kind = type_ARRAY;
    t->val.type = t1;
    return t;
}

type *make_Type_record(var_decl_list *vdl) {
    type *t;
    t = NEW(type);
    t->lineno = lineno;
    t->kind = type_RECORD;
    t->val.list = vdl;
    return t;
}

par_decl_list *make_PDL_list(var_decl_list *vdl) {
    par_decl_list *pdl;
    pdl = NEW(par_decl_list);
    pdl->lineno = lineno;
    pdl->kind = pdl_LIST;
    pdl->list = vdl;
    return pdl;
}

par_decl_list *make_PDL_empty() {
    par_decl_list *pdl;
    pdl = NEW(par_decl_list);
    pdl->lineno = lineno;
    pdl->kind = pdl_EMPTY;
    pdl->list = NULL;
    return pdl;
}

var_decl_list *make_VDL_list(var_type *vt, var_decl_list *vdl1) {
    var_decl_list *vdl;
    vdl = NEW(var_decl_list);
    vdl->lineno = lineno;
    vdl->kind = vdl_LIST;
    vdl->list = vdl1;
    vdl->vartype = vt;
    return vdl;
}

var_decl_list *make_VDL_type(var_type *vt) {
    var_decl_list *vdl;
    vdl = NEW(var_decl_list);
    vdl->lineno = lineno;
    vdl->kind = vdl_TYPE;
    vdl->list = NULL;
    vdl->vartype = vt;
    return vdl;
}

var_type *make_VType_id(char *id, type *t) {
    var_type *vt;
    vt = NEW(var_type);
    vt->lineno = lineno;
    vt->id = id;
    vt->type = t;
    return vt;
}

body *make_Body(decl_list *dl, statement_list *sl) {
    body *b;
    b = NEW(body);
    b->lineno = lineno;
    b->d_list = dl;
    b->s_list = sl;
    return b;
}

decl_list *make_DL_list(declaration *d, decl_list *dl1) {
    decl_list *dl;
    dl = NEW(decl_list);
    dl->lineno = lineno;
    dl->kind = dl_LIST;
    dl->decl = d;
    dl->list = dl1;
    return dl;
}

decl_list *make_DL_empty() {
    decl_list *dl;
    dl = NEW(decl_list);
    dl->lineno = lineno;
    dl->kind = dl_EMPTY;
    dl->decl = NULL;
    dl->list = NULL;
    return dl;
}

declaration *make_Decl_type(char *id, type *t) {
    declaration *d;
    d = NEW(declaration);
    d->lineno = lineno;
    d->kind = decl_TYPE;
    d->val.type.id = id;
    d->val.type.type = t;
    return d;
}

declaration *make_Decl_func(function *f) {
    declaration *d;
    d = NEW(declaration);
    d->lineno = lineno;
    d->kind = decl_FUNC;
    d->val.function = f;
    return d;
}

declaration *make_Decl_list(var_decl_list *vdl) {
    declaration *d;
    d = NEW(declaration);
    d->lineno = lineno;
    d->kind = decl_VAR;
    d->val.list = vdl;
    return d;
}

statement_list *make_SL_statement(statement *s) {
    statement_list *sl;
    sl = NEW(statement_list);
    sl->lineno = lineno;
    sl->kind = sl_STATEMENT;
    sl->statement = s;
    sl->list = NULL;
    return sl;
}

statement_list *make_SL_list(statement *s, statement_list *sl1) {
    statement_list *sl;
    sl = NEW(statement_list);
    sl->lineno = lineno;
    sl->kind = sl_LIST;
    sl->statement = s;
    sl->list = sl1;
    return sl;
}

statement *make_STMT_ret(expression *e) {
    statement *s;
    s = NEW(statement);
    s->lineno = lineno;
    s->kind = statement_RETURN;
    s->val.ret = e;
    s->contains_ret = 0;
    return s;
}

statement *make_STMT_wrt(expression *e) {
    statement *s;
    s = NEW(statement);
    s->lineno = lineno;
    s->kind = statement_WRITE;
    s->val.wrt = e;
    return s;
}

statement *make_STMT_allocate_var(variable *v) {
    statement *s;
    s = NEW(statement);
    s->lineno = lineno;
    s->kind = statement_ALLOCATE;
    s->val.allocate.variable = v;
    s->val.allocate.length = NULL;
    return s;
}

statement *make_STMT_allocate_length(variable *v, expression *e) {
    statement *s;
    s = NEW(statement);
    s->lineno = lineno;
    s->kind = statement_ALLOCATE_LENGTH;
    s->val.allocate.variable = v;
    s->val.allocate.length = e;
    return s;
}

statement *make_STMT_assign(variable *v, expression *e) {
    statement *s;
    s = NEW(statement);
    s->lineno = lineno;
    s->kind = statement_ASSIGNMENT;
    s->val.assignment.variable = v;
    s->val.assignment.expression = e;
    return s;
}

statement *make_STMT_if(expression *e, statement *s1) {
    statement *s;
    s = NEW(statement);
    s->lineno = lineno;
    s->kind = statement_IF;
    s->val.ifthen.expression = e;
    s->val.ifthen.statement1 = s1;
    s->val.ifthen.statement2 = NULL;
    return s;
}

statement *make_STMT_if_else(expression *e, statement *s1, statement *s2) {
    statement *s;
    s = NEW(statement);
    s->lineno = lineno;
    s->kind = statement_IF_ELSE;
    s->val.ifthen.expression = e;
    s->val.ifthen.statement1 = s1;
    s->val.ifthen.statement2 = s2;
    return s;
}

statement *make_STMT_while(expression *e, statement *s1) {
    statement *s;
    s = NEW(statement);
    s->lineno = lineno;
    s->kind = statement_WHILE;
    s->val.loop.expression = e;
    s->val.loop.statement = s1;
    return s;
}

statement *make_STMT_list(statement_list *sl) {
    statement *s;
    s = NEW(statement);
    s->lineno = lineno;
    s->kind = statement_LIST;
    s->val.list = sl;
    return s;
}

expression *make_EXP(EXP_kind kind, expression *left, expression *right) {
    expression *e;
    e = NEW(expression);
    e->lineno = lineno;
    e->kind = kind;
    e->val.ops.left = left;
    e->val.ops.right = right;
    return e;
}

expression *make_EXP_term(term *term) {
    expression *e;
    e = NEW(expression);
    e->lineno = lineno;
    e->kind = exp_TERM;
    e->val.term = term;
    return e;
}

//Negation of x is the same as 0-x, so we make a minus node
expression *make_EXP_neg(expression *neg) {

    expression *zero = make_EXP_term(make_Term_num(0));

    expression *minus = make_EXP(exp_MIN, zero, neg);

    return minus;
}

term *make_Term_num(int intconst) {
    term *t;
    t = NEW(term);
    t->lineno = lineno;
    t->kind = term_NUM;
    t->val.num = intconst;
    return t;
}

term *make_Term_par(expression *expression) {
    term *t;
    t = NEW(term);
    t->lineno = lineno;
    t->kind = term_PAR;
    t->val.expression = expression;
    return t;
}

term *make_Term_not(term *term) {
    struct term *t;
    t = NEW(struct term);
    t->lineno = lineno;
    t->kind = term_NOT;
    t->val.term_not = term;
    return t;
}

term *make_Term_abs(expression *expression) {
    term *t;
    t = NEW(term);
    t->lineno = lineno;
    t->kind = term_ABS;
    t->val.expression = expression;
    return t;
}

term *make_Term_boolean(int bool) {
    term *t;
    t = NEW(term);
    t->lineno = lineno;
    if (bool == 1) {
        t->kind = term_TRUE;
        return t;
    }
    t->kind = term_FALSE;
    return t;
}

term *make_Term_null() {
    term *t;
    t = NEW(term);
    t->lineno = lineno;
    t->kind = term_NULL;
    return t;
}

term *make_Term_variable(variable *var) {
    term *t;
    t = NEW(term);
    t->lineno = lineno;
    t->kind = term_VAR;
    t->val.variable = var;
    return t;
}

term *make_Term_list(char *id, act_list *list) {
    term *t;
    t = NEW(term);
    t->lineno = lineno;
    t->kind = term_LIST;
    t->val.list.id = id;
    t->val.list.list = list;
    return t;
}

act_list *make_Act_list(exp_list *list) {
    act_list *al;
    al = NEW(act_list);
    al->lineno = lineno;
    al->kind = al_LIST;
    al->list = list;
    return al;
}
act_list *make_Act_empty() {
    act_list *al;
    al = NEW(act_list);
    al->lineno = lineno;
    al->kind = al_EMPTY;
    al->list = NULL;
    return al;
}

exp_list *make_ExpL_exp(expression *expression) {
    exp_list *el;
    el = NEW(exp_list);
    el->lineno = lineno;
    el->kind = el_EXP;
    el->expression = expression;
    el->list = NULL;
    return el;
}

exp_list *make_ExpL_list(expression *expression, exp_list *list) {
    exp_list *el;
    el = NEW(exp_list);
    el->lineno = lineno;
    el->kind = el_LIST;
    el->expression = expression;
    el->list = list;
    return el;
}

variable *make_Var_id(char *id) {
    variable *v;
    v = NEW(variable);
    v->lineno = lineno;
    v->kind = var_ID;
    v->id = id;
    return v;
}

variable *make_Var_exp(variable *var, expression *expression) {
    variable *v;
    v = NEW(variable);
    v->lineno = lineno;
    v->kind = var_EXP;
    v->val.exp.var = var;
    v->val.exp.exp = expression;
    return v;
}

variable *make_Var_record(variable *var, char *id) {
    variable *v;
    v = NEW(variable);
    v->lineno = lineno;
    v->kind = var_RECORD;
    v->val.record.var = var;
    v->val.record.id = id;
    return v;
}

int check_Func(head *head, tail *tail) {
    if (strcmp(head->id, tail->id) == 0) {
        return 0;
    }
    return 1;
}
