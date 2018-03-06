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

typedef struct linked_list {
  int index;
  void* data;
  struct linked_list *prev;
  struct linked_list *next;
} linked_list;

asm_node *new_asm_node(int linenumber, ASM_kind kind, char *arg1, char *arg2, char *arg3, char *comment);

void linked_list_insert_head(linked_list *list, void* node);

void linked_list_insert_tail(linked_list *list, void* node);

#endif //COMPILER_LINKED_LIST_H