#include <stdlib.h>
#include "Linked_List.h"

void add(struct Linked_List* ll_list, long num)
{
	struct Linked_List_Node* ll_node = malloc(sizeof(struct Linked_List_Node));
	ll_node->num = num;
	
	if(ll_list->first == NULL && ll_list->last == NULL)
	{
		ll_list->first = ll_node;
		ll_list->last = ll_node;
		ll_node->prev = NULL;
		ll_node->next = NULL;
	}
	else
	{
		ll_list->last->next = ll_node;
		ll_node->prev = ll_list->last;
		ll_list->last = ll_node;
		ll_node->next = NULL;
	}
}

void free_list(struct Linked_List* ll_list)
{
	struct Linked_List_Node* next_to_free = NULL;
	while(ll_list->last != NULL)
	{
		next_to_free = ll_list->last->prev;
		free(ll_list->last);
		ll_list->last = next_to_free;
	}
	
	ll_list->first = NULL;
	ll_list->last = NULL;
}
