#include <stdio.h>
#include <stdlib.h>
#include "setup.h"
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

}

void setup_head(head *head, SymbolTable *table){

}

void setup_tail(tail *tail, SymbolTable *table){

}

void setup_type(type *type, SymbolTable *table){

}

void setup_pdl(par_decl_list *pdl, SymbolTable *table){
    
}

void setup_vdl(var_decl_list *vdl, SymbolTable *table){
    
}

void setup_dlist(decl_list *dlist, SymbolTable *table){

    if (dlist->kind != dl_EMPTY){
        setup_decl(dlist->decl, table);
        setup_dlist(dlist->list, table);
    }
    
}

void setup_decl(declaration *decl, SymbolTable *table){


    
}

void setup_slist(statement_list *slist, SymbolTable *table){

    slist->table = table;
    if (slist->list != NULL){
        setup_slist(slist->list, table);
    }
    setup_stmt(slist->statement, table);
    
}

void setup_stmt(statement *stmt, SymbolTable *table){
    
}

void setup_var(variable *var, SymbolTable *table){
    
}

void setup_exp(expression *exp, SymbolTable *table){

}

void setup_term(term *term, SymbolTable *table){

}

void setup_alist(act_list *alist, SymbolTable *table){

}

void setup_elist(exp_list *elist, SymbolTable *table){

}