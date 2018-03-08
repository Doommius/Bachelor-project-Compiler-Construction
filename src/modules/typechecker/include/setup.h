#ifndef __setup_h
#define __setup_h
#include "tree.h"
#include "symbol.h"

void setup_body(body *body, symbol_table *table);

void setup_function(function *function, symbol_table *table);

void setup_head(head *head, symbol_table *table);

void setup_tail(tail *tail, symbol_table *table);

void setup_type(type *type, symbol_table *table);

void setup_pdl(par_decl_list *pdl, symbol_table *table);

void setup_vdl(var_decl_list *vdl, symbol_table *table);

void setup_dlist(decl_list *dlist, symbol_table *table);

void setup_decl(declaration *decl, symbol_table *table);

void setup_slist(statement_list *slist, symbol_table *table);

void setup_stmt(statement *stmt, symbol_table *table);

void setup_var(variable *var, symbol_table *table);

void setup_exp(expression *exp, symbol_table *table);

void setup_term(term *term, symbol_table *table);

void setup_alist(act_list *alist, symbol_table *table);

void setup_elist(exp_list *elist, symbol_table *table);


#endif