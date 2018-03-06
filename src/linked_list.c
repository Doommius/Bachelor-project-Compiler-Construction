#include "malloc.h"
#include "linked_list.h"

//returns id:

linked_list *linked_list_new_node(char *content) {
    linked_list *newelement = malloc(sizeof(struct linked_list));
    newelement->data = content;
    newelement->prev = NULL;
    newelement->next = NULL;
    return newelement;
}

void linked_list_insert_head(char *content) {
    linked_list *newelement = linked_list_new_node(content);
    if (linked_list_head == NULL) {
        linked_list_head = newelement;
        return;
    }
    linked_list_head->prev = newelement;
    newelement->next = linked_list_head;
    linked_list_head = newelement;
}

void linked_list_insert_tail(char *content) {
    linked_list *temp = linked_list_head;
    linked_list *newelement = linked_list_new_node(content);
    if (linked_list_head == NULL) {
        linked_list_head = newelement;
        return;
    }
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newelement;
    newelement->prev = temp;
}