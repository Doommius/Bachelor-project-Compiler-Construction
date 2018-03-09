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

linked_list *init_linked_list(void *data) {
    linked_list_meta *meta = malloc(sizeof(linked_list_meta));
    linked_list *list = malloc(sizeof(linked_list));

    list->data = data;
    list->meta = meta;
    list->next = list;
    list->prev = list;

    meta->length = 1;
    meta->head = list;
    meta->tail = list;
    meta->current_pos = 0;
    meta->current_element = list;
    return list;
}

/**
 * @brief Adds an element to the front of the linked list list_ref
 * 
 * @param list_ref is a reference pointer to the list pointer
 * @param data the data to save at that position
 * @return linked_list* 
 */
linked_list *linked_list_insert_head(linked_list **list_ref, void *data) {

    if (list_ref == NULL) {
        return init_linked_list(data);
    }

    linked_list *new_element = malloc(sizeof(linked_list));
    new_element->data = data;

    new_element->next = (*list_ref)->meta->head;
    new_element->prev = (*list_ref)->meta->tail;
    new_element->meta = (*list_ref)->meta;

    (*list_ref)->meta->tail->next = new_element;
    (*list_ref)->meta->head = new_element;
    new_element->prev->next = new_element;

    ++(*list_ref)->meta->length;
    (*list_ref) = new_element;
    return new_element;
}

void linked_list_insert_tail(linked_list *list, void *data) {

    if (list == NULL) {
        return init_linked_list(data);
    }

    linked_list *new_element = malloc(sizeof(linked_list));
    new_element->data = data;

    new_element->next = list->meta->head;
    new_element->prev = list->meta->tail;
    new_element->meta = list->meta;

    list->meta->head->prev = new_element;
    list->meta->tail = new_element;
    new_element->prev->next = new_element;

    ++list->meta->length;
}

linked_list *linked_list_get_head(linked_list *list) {
    return list->meta->head;
}

linked_list *linked_list_get_tail(linked_list *list) {
    return list->meta->tail;
}

unsigned int linked_list_length(linked_list *list) {
    return list->meta->length;
}

linked_list *linked_list_get(linked_list *list, int index) {
    linked_list *pseudo_list = list;
    int move;
    int current_pos = list->meta->current_pos;
    int length = list->meta->length;

    if (index == 0) {
        list->meta->current_element = list->meta->head;
        list->meta->current_pos = 0;
        printf("A move: 0\n");
        return list->meta->current_element;
    } else if (index == length) {
        list->meta->current_element = list->meta->tail;
        list->meta->current_pos = index;
        printf("B move: 0\n");
        return list->meta->current_element;
    } else if (index == current_pos) {
        printf("C move: 0\n");
        return list->meta->current_element;
    } else if (index > current_pos && length - index >= index - current_pos ) {
		move = abs(index - current_pos);
		printf("D move: %i\n", move);
		return linked_list_iterator(move, index, list, pseudo_list->meta->current_element, 0);
	} else if (index <= current_pos && length - index <= index - current_pos ) {
		move = abs(index - current_pos);
		printf("E move: %i\n", move);
		return linked_list_iterator(move, index, list, pseudo_list->meta->current_element, 1);
	} else if (index <= current_pos / 2) {
		move = abs(index);
		printf("F move: %i\n", move);
		return linked_list_iterator(move, index, list, pseudo_list->meta->head, 0);
	} else if (index > current_pos / 2 && index < current_pos) {
		move = abs(index - current_pos);
		printf("G move: %i\n", move);
		return linked_list_iterator(move, index, list, pseudo_list->meta->current_element, 1);
	}  else if (index > current_pos && length - index < index - current_pos / 2) {
		move = abs(length - index -1);
		printf("H move: %i\n", move);
		return linked_list_iterator(move, index, list, pseudo_list->meta->tail, 1);
	}
}


linked_list *linked_list_iterator(int move, int index, linked_list *list, linked_list *pseudo_list, int d) {
	switch(d) {
		case 0:
			for (int i = 0; i < move; ++i) {
				pseudo_list = pseudo_list->next;
			}
			break;
		case 1:
			for (int i = 0; i < move; ++i) {
				pseudo_list = pseudo_list->prev;
			}
			break;
	}
	list->meta->current_pos = index;
	list->meta->current_element = pseudo_list;
	return pseudo_list;
}