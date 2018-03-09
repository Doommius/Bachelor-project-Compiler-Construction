// First file!
#include <stdio.h>
#include <string.h>
#include "symbol.h"
#include "symboltabletest.h"
#include "linked_list.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define testString "kitty"
#define testString2 "tesu"
#define testString3 "tets"

int main(int argc, char **argv) {
    //symboltable_test();
	linked_list *list = init_linked_list("Hej");

	linked_list *head;

	// printf("HEAD: %s  TAIL: %s\n", list->meta->head->data, list->meta->tail->data );
	// linked_list_insert_head(&list, "Hoved 1");


	printf("HEAD: %s  TAIL: %s\n", list->meta->head->data, list->meta->tail->data );
	// printf("- -%s\n", list->meta->head->data);
	// printf("- -%s\n", list->meta->tail->data);

	// printf("Test %s\n", list->data);

	// printf("Loop start\n");
	linked_list_insert_tail(list, "Hale 1");



	// printf("HEAD: %s  TAIL: %s\n", list->meta->head->data, list->meta->tail->data );

	// head = linked_list_insert_head(&list, "Hoved 2");

	printf("HEAD: %s  TAIL: %s\n", list->meta->head->data, list->meta->tail->data );

	printf("HEAD: %s  TAIL: %s\n", list->meta->head->data, list->meta->tail->data );
	linked_list_insert_tail(list, "Hale 2");
	linked_list_insert_tail(list, "Hale 3");
	linked_list_insert_tail(list, "Hale 4");
	linked_list_insert_tail(list, "Hale 5");
	linked_list_insert_tail(list, "Hale 6");
	linked_list_insert_tail(list, "Hale 7");
	linked_list_insert_tail(list, "Hale 8");
	linked_list_insert_tail(list, "Hale 9");

	// printf("---%s\n", list->meta->head->data);
	// printf("---%s\n", list->meta->tail->data);
	
	// printf("--%s\n", list->data);
	// printf("--%s\n", list->next->data);
	// printf("--%s\n", list->next->next->data);
	// printf("--%s\n", list->next->next->next->data);
	// printf("--%s\n", list->next->next->next->next->data);
	// printf("--%s\n", list->next->next->next->next->next->data);
	
	// printf("---%s\n", list->next->next->next->next->prev->data);
	

	printf("\nLoop start\n");
	// while(list->next != list->meta->head) {
	// 	printf("%s\n", list->data);
	// 	list = list->next;
	// }
	// list = head;
	for(int i = 0; i < 20; ++i) {
		printf("%s\n\n", list->data);
		list = list->next;
	}

	printf("Length: %i\n\n", list->meta->length);

	for(int i = 0; i < list->meta->length; ++i) {
		printf("GET %i: %s\n\n", i, linked_list_get(list, i)->data);
	}
	printf("GET 0: %s\n\n", linked_list_get(list, 0)->data);
	printf("GET 6: %s\n\n", linked_list_get(list, 6)->data);
}