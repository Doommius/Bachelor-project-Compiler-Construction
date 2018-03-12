#ifndef __pretty_h
#define __pretty_h

#include "tree.h"

void prettyFunc(function *f);

void prettyHead(head *h);

void prettyTail(tail *t);

void prettyType(type *t);

void prettyPDL(par_decl_list *pdl);

void prettyVDL(var_decl_list *vdl);

void prettyVT(var_type *vt);

void prettyBody(body *b);

void prettyDL(decl_list *dl);

void prettyDecl(declaration *d);

void prettySL(statement_list *sl);

void prettySTMT(statement *s);

void prettyVar(variable *v);

void prettyEXP(expression *e);

void prettyTerm(term *t);

void prettyAL(act_list *al);

void prettyEL(exp_list *el);

void indent();

void prettySymbol(symbol_table *table, char *id, int line);

void prettyStype(symbol_type *stype, int line); 

#endif