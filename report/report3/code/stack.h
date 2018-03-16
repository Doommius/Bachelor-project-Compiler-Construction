#ifndef __stack_h
#define __stack_h

typedef struct stack_node{
    void *val;
    struct stack_node *next;
} stack_node;

typedef struct stack{
    struct stack_node *top;
} stack;

stack *init_stack();

void stack_push(stack *stack, void *val);

void *stack_pop(stack *stack);

void *stack_read(stack *stack);

#endif