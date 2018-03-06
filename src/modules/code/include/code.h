#ifndef __code_h
#define __code_h

#include "tree.h"
#include "linked_list.h"

void eval_statement(linked_list *list,statement *s);

void eval_expression(linked_list *list,expression *e);

void code_GEQ_statement(linked_list *list,expression *e);

void code_or_expression(linked_list *list,expression *e);

void code_and_expression(linked_list *list,expression *e);

void code_general_expression(linked_list *list,expression *e);

linked_list *buildcode(statement_list input);

#endif