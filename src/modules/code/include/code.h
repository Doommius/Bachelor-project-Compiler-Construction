#ifndef __code_h
#define __code_h

#include "tree.h"
#include "linked_list.h"

void code_or_expression(expression *e);

void code_and_expression(expression *e);

void code_general_expression(expression *e);

void code_GEQ_statement(expression *e);

void code_LEQ_statement(expression *e);

void code_LT_statement(expression *e);

void code_GT_statement(expression *e);

void code_NEQ_statement(expression *e);

void code_EQ_statement(expression *e);

void eval_expression(expression *e);

void eval_statement(statement *s);

linked_list *buildcode(statement_list input);

#endif