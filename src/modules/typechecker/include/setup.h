#ifndef __setup_h
#define __setup_h
#include "tree.h"
#include "symbol.h"

void setup_body(body *body, SymbolTable *table);

void setup_function(function *function, SymbolTable *table);

void setup_head(head *head, SymbolTable *table);

void setup_tail(tail *tail, SymbolTable *table);

void setup_type(type *type, SymbolTable *table);

void setup_pdl(par_decl_list *pdl, SymbolTable *table);

void setup_vdl(var_decl_list *vdl, SymbolTable *table);

void setup_vtype(var_type *vtype, SymbolTable *table);

void setup_dlist(decl_list *dlist, SymbolTable *table);

void setup_decl(declaration *decl, SymbolTable *table);

void setup_slist(statement_list *slist, SymbolTable *table);

void setup_stmt(statement *stmt, SymbolTable *table);

void setup_var(variable *var, SymbolTable *table);

void setup_exp(expression *exp, SymbolTable *table);

void setup_term(term *term, SymbolTable *table);

void setup_alist(act_list *alist, SymbolTable *table);

void setup_elist(exp_list *elist, SymbolTable *table);


#endif