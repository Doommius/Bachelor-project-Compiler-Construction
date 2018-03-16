#ifndef __check_h
#define __check_h
#include "tree.h"
#include "symbol.h"


void check_body(body *body);

void check_function(function *function);

void check_dlist(decl_list *dlist);

void check_decl(declaration *decl);

void check_slist(statement_list *slist);

void check_stmt(statement *stmt);

void check_var(variable *var);

void check_exp(expression *exp);

void check_term(term *term);

void check_alist(act_list *alist);

void check_elist(exp_list *elist);

int check_function_args(par_decl_list *pdl, act_list *alist);

int compare_stype(symbol_type *stype1, symbol_type *stype2);

int compare_record(symbol_type *stype1, symbol_type *stype2);



#endif