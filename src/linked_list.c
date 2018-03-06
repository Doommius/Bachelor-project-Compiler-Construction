#include "malloc.h"
#include "linked_list.h"

//returns id:

asm_node *new_asm_node(int linenumber, ASM_kind kind, char *arg1, char *arg2, char *arg3, char *comment) {
    asm_node *node = malloc(sizeof(struct asm_node));
    node->linenumber = linenumber;
    node->kind = kind;
    node->arg1 = arg1;
    node->arg2 = arg2;
    node->arg3 = arg3;
    node->comment = comment;
    return node;
}

void linked_list_insert_head(linked_list *list, void *node) {
    linked_list *newelement = malloc(sizeof(linked_list));
    newelement->data = node;

    if (list == NULL) {
        list = newelement;
        list->next = list;
        list->prev = list;
        return;
    }
    newelement->prev = list->prev;
    list->prev = newelement;
    newelement->next = list;
    list = newelement;
}

void linked_list_insert_tail(linked_list *list, void *node) {
    linked_list *temp = list;
    linked_list *newelement = malloc(sizeof(linked_list));
    newelement->data = node;
    if (list == NULL) {
        list = newelement;
        list->next = list;
        list->prev = list;
        return;
    }
    temp->prev->next = newelement;
    newelement->prev = temp->prev;
    temp->prev = newelement;
    newelement->next = temp;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newelement;
    newelement->prev = temp;
}