#ifndef __weeder_h
#define __weeder_h

#include "tree.h"

body *weeder_init(body *program);

body *weeder(body *body);

body *weed_body(body *body);

function *weed_function(function *func);

head *weed_head(head *head);

tail *weed_tail(tail *tail);

type *weed_type(type *type);

par_decl_list *weed_pdl(par_decl_list *pdl);

var_decl_list *weed_vdl(var_decl_list *vdl);

var_type *weed_vtype(var_type *vtype);

decl_list *weed_dlist(decl_list *dlist);

declaration *weed_decl(declaration *decl);

statement_list *weed_slist(statement_list *slist);

statement *weed_stmt(statement *stmt);

variable *weed_variable(variable *variable);

expression *weed_expression(expression *expression);

term *weed_term(term *term);

act_list *weed_alist(act_list *alist);

exp_list *weed_elist(exp_list *elist);




#endif