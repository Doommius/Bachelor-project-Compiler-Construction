#include "tree.h"


char *eval_statement(statement *s);

char *eval_expression(expression *e);

char *code_GQ_statement(expression *e);

char *code_or_expression(expression *e);

char *code_and_expression(expression *e);

