//
// Created by jervelund on 3/6/18.
//

#ifndef __linked_list_h
#define __linked_list_h




typedef struct linked_list {
    int id;
    char  *data;
    struct linked_list *prev;
    struct linked_list *next;
} linked_list;


linked_list *linked_list_head;


linked_list* makeNewNode(char *content);
void Linked_List_InsertAtHead(char *content);
void Linked_List_InsertAtTail(char *content);



#endif //COMPILER_LINKED_LIST_H