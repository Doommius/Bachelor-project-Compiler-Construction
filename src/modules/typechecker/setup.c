#include <stdio.h>
#include <stdlib.h>
#include "setup.h"
#include "symbol.h"
#include "memory.h"

/**
 * 
 * TODO All of this can possibly be put in the "tree.c", when we create a node. 
 * This means that we do not have to run through the AST again to set up the symbol table
 * 
 */


void setup_body(body *body, SymbolTable *table){

    body->table = table;

    setup_dlist(body->d_list, table);

    setup_slist(body->s_list, table);

}



void setup_function(function *function, SymbolTable *table){

    SymbolTable *nextTable;
    nextTable = scopeSymbolTable(table);
    function->table = nextTable;
    function->tail->table = nextTable;
    setup_head(function->head, table);
    setup_body(function->body, nextTable);





}

void setup_head(head *head, SymbolTable *table){

}

void setup_tail(tail *tail, SymbolTable *table){

}

void setup_type(type *type, SymbolTable *table){
    type->table = table;


}

void setup_pdl(par_decl_list *pdl, SymbolTable *table){
    pdl->table = table;
    
}

void setup_vdl(var_decl_list *vdl, SymbolTable *table){
    
    vdl->table = table;

    
}

void setup_vtype(var_type *vtype, SymbolTable *table){

    vtype->table = table;


}

void setup_dlist(decl_list *dlist, SymbolTable *table){

    if (dlist->kind != dl_EMPTY){
        setup_decl(dlist->decl, table);
        setup_dlist(dlist->list, table);
    }
    
}

void setup_decl(declaration *decl, SymbolTable *table){

    decl->table = table;
    switch (decl->kind){

        case (decl_TYPE):
            symbolType *st;
            st = NEW(symbolType);
            st->type = symbol_ID;
            putSymbol(table, decl->val.type.id, 0, st);
            setup_type(decl->val.type.type, table);
            break;

        case (decl_FUNC):
            setup_function(decl->val.function, table);
            break;

        case (decl_VAR):
            setup_vdl(decl->val.list, table);
            break;

    }
    
}

void setup_slist(statement_list *slist, SymbolTable *table){

    slist->table = table;
    if (slist->list != NULL){
        setup_slist(slist->list, table);
    }
    setup_stmt(slist->statement, table);
    
}

void setup_stmt(statement *stmt, SymbolTable *table){

    stmt->table = table;
    switch(stmt->kind){

        case (statement_RETURN):
            setup_exp(stmt->val.ret, table);
            break;
        
        case (statement_WRITE):
            setup_exp(stmt->val.wrt, table);
            break;

        case (statement_ALLOCATE):
            setup_var(stmt->val.allocate.variable, table);
            break;
        
        case (statement_ALLOCATE_LENGTH):
            setup_var(stmt->val.allocate.variable, table);
            setup_exp(stmt->val.allocate.length, table);
            break;

        case (statement_ASSIGNMENT):
            setup_var(stmt->val.assignment.variable, table);
            setup_exp(stmt->val.assignment.expression, table);
            break;

        case (statement_IF):
            setup_exp(stmt->val.ifthen.expression, table);
            setup_stmt(stmt->val.ifthen.statement1, table);
            break;

        case (statement_IF_ELSE):
            setup_exp(stmt->val.ifthen.expression, table);
            setup_stmt(stmt->val.ifthen.statement1, table);
            setup_stmt(stmt->val.ifthen.statement2, table);
            break;

        case (statement_WHILE):
            setup_exp(stmt->val.loop.expression, table);
            setup_stmt(stmt->val.loop.statement, table);
            break;

        case (statement_LIST):
            setup_slist(stmt->val.list, table);
            break;
    }
    
}

void setup_var(variable *var, SymbolTable *table){

    var->table = table;
    if (var->kind == var_EXP){
        setup_var(var->val.exp.var, table);
        setup_exp(var->val.exp.exp, table);
    } else {
        setup_var(var->val.record.var, table);
    }
    
}

void setup_exp(expression *exp, SymbolTable *table){

    exp->table = table;
    if (exp->kind == exp_TERM){
        setup_term(exp->val.term, table);
    } else {
        setup_exp(exp->val.ops.left, table);
        setup_exp(exp->val.ops.right, table);
    }

}

void setup_term(term *term, SymbolTable *table){

    term->table = table;
    switch(term->kind){

        case (term_VAR):
            setup_var(term->val.variable, table);
            break;

        case (term_LIST):
            setup_alist(term->val.list.list,  table);
            break;

        case (term_PAR):
            setup_exp(term->val.expression, table);
            break;

        case (term_NOT):
            setup_term(term->val.term_not, table);
            break;

        case (term_ABS):
            setup_exp(term->val.expression, table);
            break;

        default:
            break;

    }

}

void setup_alist(act_list *alist, SymbolTable *table){

    alist->table = table;
    if (alist->kind != al_EMPTY){
        setup_elist(alist->list, table);
    }

}

void setup_elist(exp_list *elist, SymbolTable *table){
    elist->table = table;
    setup_exp(elist->expression, table);
    if (elist->table != NULL){
        setup_elist(elist->list, table);
    }
}