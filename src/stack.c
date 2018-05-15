#include <stdio.h>
#include "stack.h"
#include "memory.h"

stack *init_stack() {

	stack *s;
	s = NEW(stack);
	s->top = NULL;
	return s;
}

void stack_push(stack *stack, void *val) {

	stack_node *sn;
	sn = NEW(stack_node);
	sn->val = val;
	sn->next = stack->top;
	stack->top = sn;
}

void *stack_pop(stack *stack) {

	stack_node *top;
	void *val;

	top = stack->top;
	val = top->val;
	stack->top = top->next;
	free(top);
	return val;
}

void *stack_read(stack *stack) {

	if (stack->top == NULL) {
		return NULL;
	}
	return stack->top->val;
}