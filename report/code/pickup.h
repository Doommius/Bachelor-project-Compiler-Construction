#ifndef __pickup_h
#define __pickup_h
#include "tree.h"

void pickup_body(body *body);

void pickup_function(function *function);

void pickup_head(head *head);

void pickup_pdl(par_decl_list *pdl);

void pickup_vdl(var_decl_list *vdl);

void pickup_vtype(var_type *vtype);

void pickup_dlist(decl_list *dlist);

void pickup_declaration(declaration *decl);

void pickup_type(type *type);

type *resolve_recursive_type(type *type);

#endif