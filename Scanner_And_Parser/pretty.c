#include <stdio.h>
#include "pretty.h"
#include "tree.h"

void prettyFunc(function *f){
    prettyHead(f->head);
    prettyBody(f->body);
    prettyTail(f->tail);
}

void prettyHead(head *h){
    printf("func %s (", h->id);
    prettyPDL(h->list);
    printf(") :");
    prettyType(h->type);
}

void prettyTail(tail *t){
    printf("\nend %s", t->id);
}

void prettyType(type *t){
    switch (t->kind){
        case type_ID:
            printf("%s", t->val.id);
            break;

        case type_INT:
            printf("int");
            break;

        case type_BOOl:
            printf("bool");
            break;

        case type_ARRAY:
            printf("array of");
            break;

        case type_RECORD:
            printf("record of");
            prettyVDL(t->val.list);
            break;
    }
}

void prettyPDL(par_decl_list *pdl){
    switch (pdl->kind){
        case pdl_LIST:
            prettyVDL(pdl->list);
            break;

        case pdl_EMPTY:
            break;
    }
}

void prettyVDL(var_decl_list *vdl){
    switch (vdl->kind){
        case vdl_LIST:
            prettyVT(vdl->vartype);
            printf(",");
            prettyVDL(vdl->list);
            break;

        case vdl_TYPE:
            prettyVT(vdl->vartype);
            break;
    }
}

void prettyVT(var_type *vt){
    printf("%s : ",vt->id);
    prettyType(vt->type);
}

void prettyBody(body *b){
    prettyDL(b->d_list);
    prettySL(b->s_list);
}

void prettyDL(decl_list *dl){
    switch (dl->kind){
        case dl_LIST:
            prettyDecl(dl->decl);
            prettyDL(dl->list);
            break;

        case dl_EMPTY:
            break;
    }
}

void prettyDecl(declaration *d){
    switch (d->kind){
        case decl_TYPE:
            printf("type %s = ", d->val.type.id);
            prettyType(d->val.type.type);
            printf(";\n");
            break;

        case decl_FUNC:
            prettyFunc(d->val.function);
            break;

        case decl_VAR:
            printf("var ");
            prettyVDL(d->val.list);
            printf(";\n");
            break;
    }
}

void prettySL(statement_list *sl){
    switch (sl->kind){
        case sl_STATEMENT:
            prettySTMT(sl->statement);
            break;

        case sl_LIST:
            prettySTMT(sl->statement);
            prettySL(sl->list);
    }
}

void prettySTMT(statement *s){
    switch (s->kind){
        case statement_RETURN:
            printf("return ");
            prettyEXP(s->val.ret);
            printf(";\n");
            break;

        case statement_WRITE:
            printf("write ");
            prettyEXP(s->val.wrt);
            printf(";\n");
            break;

        case statement_ALLOCATE:
            printf("allocate ");
            prettyVar(s->val.allocate.variable);
            printf(";\n");
            break;

        case statement_ALLOCATE_LENGTH:
            printf("allocate ");
            prettyVar(s->val.allocate.variable);
            printf(" of length ");
            prettyEXP(s->val.allocate.length);
            printf(";\n");
            break;

        case statement_ASSIGNMENT:
            prettyVar(s->val.assignment.variable);
            printf(" = ");
            prettyEXP(s->val.assignment.expression);
            printf(";\n");
            break;

        case statement_IF:
            printf("if ");
            prettyEXP(s->val.ifthen.expression);
            printf(" then ");
            prettySTMT(s->val.ifthen.statement1);
            break;

        case statement_IF_ELSE:
            printf("if ");
            prettyEXP(s->val.ifthen.expression);
            printf(" then ");
            prettySTMT(s->val.ifthen.statement1);
            printf(" else ");
            prettySTMT(s->val.ifthen.statement2);
            break;

        case statement_WHILE:
            printf("while ");
            prettyEXP(s->val.loop.expression);
            printf(" do ");
            prettySTMT(s->val.loop.statement);
            break;

        case statement_LIST:
            printf("{");
            prettySL(s->val.list);
            printf("}");
            break;
    }
}


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