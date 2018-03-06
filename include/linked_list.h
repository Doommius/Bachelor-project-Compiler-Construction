#ifndef __linked_list_h
#define __linked_list_h

typedef struct linked_list {
    int id;
    char *data;
    struct linked_list *prev;
    struct linked_list *next;
} linked_list;

linked_list *linked_list_head;

linked_list *linked_list_new_node(char *content);

void linked_list_insert_head(char *content);

void linked_list_insert_tail(char *content);

#endif //COMPILER_LINKED_LIST_H