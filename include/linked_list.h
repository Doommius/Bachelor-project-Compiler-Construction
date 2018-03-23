#ifndef __linked_list_h
#define __linked_list_h

#include "kind.h"


typedef struct asm_node {
    int id;
    ASM_kind kind;
    char *arg1;
    char *arg2;
    char *arg3;
    int linenumber;
    char *comment;

} asm_node;

//Abstract assembly node, with a build in linked list.
//To be expanded
typedef struct a_asm {
	ASM_kind *ins;
	struct a_asm *prev;
	struct a_asm *next;
	char *comment;
	int ops;

	/**
	 * 
	 * Should probably contain information about the head and tail of the current block
	 * 
	 */

	union {

		//Missing something for label

		struct {
			struct asm_op *op1;
			struct asm_op *op2;
		} two_op;

		struct {
			struct asm_op *op;
		} one_op;
	} val;

} a_asm;

typedef struct asm_op {
	OP_kind *type;
	int stack_offset;
	union {
		char *reg_id;
		struct {
			int id;
		} temp;
		int intconst;
	} val;
	
} asm_op;

typedef struct linked_list_meta {
	struct linked_list *head;
	struct linked_list *tail;
	int length;
	int current_pos;
	struct linked_list *current_element;
} linked_list_meta;

typedef struct linked_list {
  void* data;
  struct linked_list *prev;
  struct linked_list *next;
  struct linked_list_meta *meta;
} linked_list;

typedef enum {NEXT, PREV} direction;

asm_node *new_asm_node(int linenumber, ASM_kind kind, char *arg1, char *arg2, char *arg3, char *comment);

linked_list* init_linked_list();

linked_list* linked_list_insert_head(linked_list **list, void* node);

linked_list* linked_list_insert_tail(linked_list *list, void* node);

linked_list* linked_list_get_head(linked_list *list);

linked_list* linked_list_get_tail(linked_list *list);

linked_list* linked_list_remove_head(linked_list *list);

linked_list* linked_list_remove_tail(linked_list *list);

linked_list* linked_list_remove_at_index(linked_list *list, unsigned i);

unsigned int linked_list_length(linked_list *list);

linked_list *linked_list_get(linked_list *list, int index);

linked_list *linked_list_iterator(int move, int index,  linked_list *pseudo_list, direction d);

#endif //COMPILER_LINKED_LIST_H