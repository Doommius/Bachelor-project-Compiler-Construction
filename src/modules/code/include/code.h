#ifndef __code_h
#define __code_h

#include "tree.h"
#include "linked_list.h"

void eval_statement(statement *s);

void eval_expression(expression *e);

void code_GEQ_statement(expression *e);

void code_or_expression(expression *e);

void code_and_expression(expression *e);

linked_list *buildcode(statement_list input);

#endif