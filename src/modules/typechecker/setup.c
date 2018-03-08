#include <stdio.h>
#include <stdlib.h>
#include "setup.h"
/**
 * 
 * TODO All of this can possibly be put in the "tree.c", when we create a node. 
 * This means that we do not have to run through the AST again to set up the symbol table
 * 
 */


void setup_body(body *body, symbol_table *table){

    body->table = table;

    setup_dlist(body->d_list, table);

    setup_slist(body->s_list, table);

}



void setup_function(function *function, symbol_table *table){

}

void setup_head(head *head, symbol_table *table){

}

void setup_tail(tail *tail, symbol_table *table){

}

void setup_type(type *type, symbol_table *table){

}

void setup_pdl(par_decl_list *pdl, symbol_table *table){
    
}

void setup_vdl(var_decl_list *vdl, symbol_table *table){
    
}

void setup_dlist(decl_list *dlist, symbol_table *table){

    if (dlist->kind != dl_EMPTY){
        setup_decl(dlist->decl, table);
        setup_dlist(dlist->list, table);
    }
    
}

void setup_decl(declaration *decl, symbol_table *table){


    
}

void setup_slist(statement_list *slist, symbol_table *table){

    slist->table = table;
    if (slist->list != NULL){
        setup_slist(slist->list, table);
    }
    setup_stmt(slist->statement, table);
    
}

void setup_stmt(statement *stmt, symbol_table *table){
    
}

void setup_var(variable *var, symbol_table *table){
    
}

void setup_exp(expression *exp, symbol_table *table){

}

void setup_term(term *term, symbol_table *table){

}

void setup_alist(act_list *alist, symbol_table *table){

}

void setup_elist(exp_list *elist, symbol_table *table){

}