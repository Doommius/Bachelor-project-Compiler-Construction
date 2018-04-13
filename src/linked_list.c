/**
 * @brief Responsible for linked lists.
 * 
 * @file linked_list.c
 * @author Troels Blicher Petersen
 * @date 2018-03-09
 */

#include "malloc.h"
#include "linked_list.h"

/**
 * @brief Creates a node for assembly language generation
 * 
 * @param linenumber 
 * @param kind What kind of operation.
 * @param arg1 
 * @param arg2 
 * @param arg3 
 * @param comment Comment at end of line.
 * @return asm_node* Return the created node.
 */
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

/**
 * @brief Initializes the linked list with an initial data pointer as well.
 * 
 * @param data for the first element of the linked list.
 * @return linked_list* Returns a reference to the created linked list.
 */
linked_list *linked_list_init(void *data) {
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
 * @brief Adds an element to the front of the linked list list_ref.
 * 
 * @param list_ref is a reference pointer to the list pointer.
 * @param data the data to save at that position
 * @return linked_list* 
 */
linked_list *linked_list_insert_head(linked_list **list_ref, void *data) {

    if (list_ref == NULL) {
        return linked_list_init(data);
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

/**
 * @brief Inserts an element to the end/tail of the linked list.
 * 
 * @param list is a reference to the list, that has to be modified.
 * @param data the data to save at that position.
 * @return linked_list* Returns a reference to the created linked list.
 */
linked_list *linked_list_insert_tail(linked_list *list, void *data) {

    if (list == NULL) {
        return linked_list_init(data);
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
    return list;
}

/**
 * @brief retrieves the head of the list.
 * 
 * @param list reference to list, where the head has to be found.
 * @return linked_list* Returns the head of the list.
 */
linked_list *linked_list_get_head(linked_list *list) {
    return list->meta->head;
}

/**
 * @brief retrieves the tail of the list.
 * 
 * @param list reference to list, where the tail has to be found.
 * @return linked_list* returns the tail of the list.
 */
linked_list *linked_list_get_tail(linked_list *list) {
    return list->meta->tail;
}

/**
 * @brief Removes the head of the list.
 * 
 * @param list reference to list, where the head has to be removed.
 * @return linked_list* returns the new list.
 */
linked_list *linked_list_remove_head(linked_list *list) {
	linked_list *old_head = list->meta->head;
	list->meta->head = list->meta->head->next;
	list->meta->head->prev = list->meta->tail;
	list->meta->tail->next = list->meta->head;
	list->meta->length--;
	if(list->meta->current_pos == 0) {
		list->meta->current_element = list->meta->head;
	}
	free(old_head);
	return list;
}

/**
 * @brief Removes tail of the list.
 * 
 * @param list reference to list, where the head has to be removed.
 * @return linked_list* returns the new list.
 */
linked_list *linked_list_remove_tail(linked_list *list) {
	linked_list *old_tail = list->meta->tail;
	list->meta->tail = list->meta->tail->prev;
	list->meta->tail->next = list->meta->head;
	list->meta->head->prev = list->meta->tail;
	if(list->meta->current_pos == list->meta->length-1) {
		list->meta->current_element = list->meta->current_element->prev;
		list->meta->current_pos = list->meta->length-2;
	}
	list->meta->length--;
	free(old_tail);
	return list;
}

/**
 * @brief Removes the ith item from the list
 * 
 * @param list list to remove the element from.
 * @param i element at index i to remove.
 * @return linked_list* returns updated list.
 */
linked_list *linked_list_remove_at_index(linked_list *list, unsigned i) {
	if(i >= list->meta->length-1) {
		return 0;
	}

	linked_list *item = linked_list_get(list, i);

	item->prev->next = item->next;
	item->next->prev = item->prev;
	if(item == list->meta->head) {
		list->meta->head = item->next;
	} else if (item == list->meta->tail) {
		list->meta->tail = item->prev;
	}
	free(item);
	return list;
}

/**
 * @brief retrieves the length of the linked list.
 * 
 * @param list The list to find the length on.
 * @return unsigned int Returns the length on the list.
 */
unsigned int linked_list_length(linked_list *list) {
    return list->meta->length;
}

/**
 * @brief Gets the ith element of a linked list. It is optimized for low cycles,
 * especially if the element is very close to the previously gotten element.
 * 
 * @param list The list to find the ith element on.
 * @param index Index is the ith element to find.
 * @return linked_list* Returns the ith element.
 */
linked_list *linked_list_get(linked_list *list, int index) {
    linked_list *pseudo_list = list;
    int move;
    int current_pos = list->meta->current_pos;
    int length = list->meta->length;
	direction d;

	if (index > length || index < 0) {
		printf("Error: Out of bounds");
		return -1;
	} else if (index == 0) {
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
    } else if (index > current_pos && length - index >= index - current_pos) {
        move = abs(index - current_pos);
        printf("D move: %i\n", move);
        return linked_list_iterator(move, index, pseudo_list->meta->current_element, NEXT);
    } else if (index <= current_pos && length - index <= index - current_pos) {
        move = abs(index - current_pos);
        printf("E move: %i\n", move);
        return linked_list_iterator(move, index, pseudo_list->meta->current_element, PREV);
    } else if (index <= current_pos / 2) {
        move = abs(index);
        printf("F move: %i\n", move);
        return linked_list_iterator(move, index, pseudo_list->meta->head, NEXT);
    } else if (index > current_pos / 2 && index < current_pos) {
        move = abs(index - current_pos);
        printf("G move: %i\n", move);
        return linked_list_iterator(move, index, pseudo_list->meta->current_element, PREV);
    } else if (index > current_pos && index <= length / 2 + current_pos / 2) {
        move = abs(index - current_pos);
        printf("H move: %i\n", move);
        return linked_list_iterator(move, index, pseudo_list->meta->current_element, NEXT);
    } else if (index > current_pos && index > length / 2 + current_pos / 2) {
        move = abs(length - index);
        printf("I move: %i\n", move);
        return linked_list_iterator(move, index, pseudo_list->meta->head, PREV);
    }
}

/**
 * @brief Iterates the linked list in an optimal way based on the if clause it is 
 * called inside of.
 * 
 * @param move How many steps to move.
 * @param index To update the meta data about the current position.
 * @param pseudo_list The list to find the ith element and update the meta data on.
 * @param d Direction to move. NEXT or PREV.
 * @return linked_list* Return the ith element
 */
linked_list *linked_list_iterator(int move, int index,  linked_list *pseudo_list, direction d) {
    switch (d) {
    case NEXT:
        for (int i = 0; i < move; ++i) {
            pseudo_list = pseudo_list->next;
        }
        break;
    case PREV:
        for (int i = 0; i < move; ++i) {
            pseudo_list = pseudo_list->prev;
        }
        break;
    }
    pseudo_list->meta->current_pos = index;
    pseudo_list->meta->current_element = pseudo_list;
    return pseudo_list;
}

int linked_list_contains(linked_list *list, void *element) {
    linked_list *current_node = list;
    do {
        if(current_node->data == element) {
            return 1;
        }
        current_node = current_node->next;
    } while(current_node != list->meta->head);
    return 0;
}