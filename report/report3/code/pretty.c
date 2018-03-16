/**
 * @brief 
 * 
 * @file pretty.c
 * @author Morten Jæger, Mark Jervelund & Troels Blicher Petersen
 * @date 2018-03-09
 */
#include <stdio.h>
#include "pretty.h"
#include "tree.h"
#include "symbol.h"
#include "error.h"

int indent_depth;
int exp_depth;
int types;
int inside_par;

void prettyProgram(body *body){
    indent_depth = 0;
    exp_depth = 0;
    inside_par = 0;
    prettyBody(body);
}

void prettyFunc(function *f) {
    prettyHead(f->head);
    indent_depth++;
    prettyBody(f->body);
    indent_depth--;
    prettyTail(f->tail);
}

void prettyHead(head *h) {
    printf("func %s(", h->id);
    prettyPDL(h->list);
    printf(") : ");
    prettyType(h->type);
    printf("\n");
}

void prettyTail(tail *t) {
    indent();
    printf("end %s", t->id);
    if(types){
        prettySymbol(t->table, t->id, t->lineno);
    }
    printf("\n");
}

void prettyType(type *t) {
    switch (t->kind) {
    case type_ID:
        printf("%s : ", t->val.id);
        if (types){
            prettyStype(t->stype, t->lineno);
        }
        break;

    case type_INT:
        printf("int");
        break;

    case type_BOOl:
        printf("bool");
        break;

    case type_ARRAY:
        printf("array of ");
        prettyType(t->val.type);
        break;

    case type_RECORD:
        printf("record of { ");
        prettyVDL(t->val.list);
        printf(" }");
        break;
    }
}

void prettyPDL(par_decl_list *pdl) {

    if (pdl->kind == pdl_LIST){
        prettyVDL(pdl->list);
    }
}

void prettyVDL(var_decl_list *vdl) {
    switch (vdl->kind) {
        case vdl_LIST:
            prettyVT(vdl->vartype);
            printf(", ");
            prettyVDL(vdl->list);
            break;

        case vdl_TYPE:
            prettyVT(vdl->vartype);
            break;
    }
}

void prettyVT(var_type *vt) {
    printf("%s : ", vt->id);
    prettyType(vt->type);
}

void prettyBody(body *b) {
    prettyDL(b->d_list);
    prettySL(b->s_list);
}

void prettyDL(decl_list *dl) {
    switch (dl->kind) {
    case dl_LIST:
        prettyDecl(dl->decl);
        prettyDL(dl->list);
        break;

    case dl_EMPTY:
        break;
    }
}

void prettyDecl(declaration *d) {
    indent();
    switch (d->kind) {
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

void prettySL(statement_list *sl) {
    switch (sl->kind) {
    case sl_STATEMENT:
        prettySTMT(sl->statement);
        break;

    case sl_LIST:
        prettySTMT(sl->statement);
        prettySL(sl->list);
        break;
    }
}

void prettySTMT(statement *s) {

    if (s->kind != statement_LIST) {
        indent();
    }

    switch (s->kind) {
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
        printf("if (");
        prettyEXP(s->val.ifthen.expression);
        printf(") then\n");
        indent_depth++;
        prettySTMT(s->val.ifthen.statement1);
        indent_depth--;
        break;

    case statement_IF_ELSE:
        printf("if (");
        prettyEXP(s->val.ifthen.expression);
        printf(") then\n");
        indent_depth++;
        prettySTMT(s->val.ifthen.statement1);
        indent_depth--;
        indent();
        printf(" else\n");
        indent_depth++;
        prettySTMT(s->val.ifthen.statement2);
        indent_depth--;
        break;

    case statement_WHILE:
        printf("while (");
        prettyEXP(s->val.loop.expression);
        printf(") do\n");
        prettySTMT(s->val.loop.statement);
        break;

    case statement_LIST:
        indent();
        printf("{\n");
        indent_depth++;
        prettySL(s->val.list);
        indent_depth--;
        indent();
        printf("}\n");
        break;
    }
}

void prettyVar(variable *v) {
    switch (v->kind) {

    case var_ID:
        printf("%s", v->id);
        break;

    case var_EXP:
        prettyVar(v->val.exp.var);
        printf("[");
        prettyEXP(v->val.exp.exp);
        printf("]");
        break;

    case var_RECORD:
        prettyVar(v->val.record.var);
        printf(".%s", v->val.record.id);
        break;
    }
}

void prettyEXP(expression *e) {
    exp_depth++;

    if (e->kind == exp_TERM){
        prettyTerm(e->val.term);
        return;
    }

    if(exp_depth > 1 && inside_par == 0){
        printf("(");
    }
    switch (e->kind) {

    case exp_MULT:
        prettyEXP(e->val.ops.left);
        printf("*");
        prettyEXP(e->val.ops.right);
        break;

    case exp_DIV:
        prettyEXP(e->val.ops.left);
        printf("/");
        prettyEXP(e->val.ops.right);
        break;

    case exp_PLUS:
        prettyEXP(e->val.ops.left);
        printf("+");
        prettyEXP(e->val.ops.right);
        break;

    case exp_MIN:
        prettyEXP(e->val.ops.left);
        printf("-");
        prettyEXP(e->val.ops.right);
        break;

    case exp_EQ:
        prettyEXP(e->val.ops.left);
        printf(" == ");
        prettyEXP(e->val.ops.right);
        break;

    case exp_NEQ:
        prettyEXP(e->val.ops.left);
        printf(" != ");
        prettyEXP(e->val.ops.right);
        break;

    case exp_GT:
        prettyEXP(e->val.ops.left);
        printf(" > ");
        prettyEXP(e->val.ops.right);
        break;

    case exp_LT:
        prettyEXP(e->val.ops.left);
        printf(" < ");
        prettyEXP(e->val.ops.right);
        break;

    case exp_GEQ:
        prettyEXP(e->val.ops.left);
        printf(" >= ");
        prettyEXP(e->val.ops.right);
        break;

    case exp_LEQ:
        prettyEXP(e->val.ops.left);
        printf(" <= ");
        prettyEXP(e->val.ops.right);
        break;

    case exp_AND:
        prettyEXP(e->val.ops.left);
        printf(" && ");
        prettyEXP(e->val.ops.right);
        break;

    case exp_OR:
        prettyEXP(e->val.ops.left);
        printf(" || ");
        prettyEXP(e->val.ops.right);
        break;

    }
    if(exp_depth > 1 && inside_par == 0){
        printf(")");
        
    }
    exp_depth--;

    if (types){
       // printf("\nCalling printStype in expression");
        printf( " : ");
        prettyStype(e->stype, e->lineno);
    }
   
}

void prettyTerm(term *t) {
    switch (t->kind) {

    case term_VAR:
        prettyVar(t->val.variable);
        break;

    case term_LIST:
        printf("%s(", t->val.list.id);
        prettyAL(t->val.list.list);
        printf(")");
        break;

    case term_PAR:
        if (exp_depth > 1){
            printf("(");
        }
        inside_par = 1;
        prettyEXP(t->val.expression);
        inside_par = 0;

        if (exp_depth > 1){
            printf("(");
        }
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

    if (types){
        //printf("\nCalling printStype in term");
		// Haps
        printf(" : ");
        prettyStype(t->stype, t->lineno);
    }
    
}

void prettyAL(act_list *al) {
    switch (al->kind) {

    case al_LIST:
        prettyEL(al->list);
        break;

    case al_EMPTY:
        break;
    }
}

void prettyEL(exp_list *el) {
    switch (el->kind) {

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

void indent() {

    int spaces = 0;
    while (spaces < (indent_depth * 4)) {
        printf(" ");
        spaces++;
    }
}

void prettySymbol(symbol_table *table, char *id, int line){

    SYMBOL *s;
    s = get_symbol(table, id);
    if (s == NULL || s->stype == NULL){
        print_error("Symbol is not recognized", 0, line);
    }
    printf(" : ");
    prettyStype(s->stype, line);
}

void prettyStype(symbol_type *stype, int line){
   // printf("\nPrintSType of type: %d: ", stype->type);
    if (stype->printed){
        return;
    }
    stype->printed = 1;
    switch(stype->type){

        case (symbol_ID):
            prettyType(stype->val.id_type);
            break;

        case (symbol_INT):
            printf("int");
            break;

        case (symbol_BOOL):
            printf("boolean");
            break;

        case (symbol_RECORD):
            printf("record of {");
            prettyVDL(stype->val.record_type);
            printf("}");
            break;

        case (symbol_ARRAY):
            printf("array[");
            prettyStype(stype->val.array_type->stype, line);
            printf("]");
            break;

        case (symbol_FUNCTION):
            printf("function(");
            prettyPDL(stype->val.func_type.pdl);
            printf(") : ");
            prettyStype(stype->val.func_type.ret_type->stype, line);
            break;
        
        case (symbol_NULL):
            printf("NULL");
            break;

        // Should never happen
        case (symbol_UNKNOWN):
            printf("unknown");
            print_error("Unknown symbol type", 0, line);
            break;

    }
    stype->printed = 0;

}