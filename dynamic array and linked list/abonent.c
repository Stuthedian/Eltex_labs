#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abonent.h"

void add(struct Abonent_List* abon_list)
{
	struct Abonent abon;

	puts("Enter name:");
	scanf("%20s", abon.name);

	puts("Enter surname:");
	scanf("%20s", abon.surname);

	puts("Enter number:");
	scanf("%11s", abon.number);

	struct Abonent_Node* abon_node = malloc(sizeof(struct Abonent_Node));
	strcpy(abon_node->abonent.name, abon.name);
	strcpy(abon_node->abonent.surname, abon.surname);
	strcpy(abon_node->abonent.number, abon.number);
	
	if(abon_list->first == NULL && abon_list->last == NULL)
	{
		abon_list->first = abon_node;
		abon_list->last = abon_node;
		abon_node->prev = NULL;
		abon_node->next = NULL;
	}
	else
	{
		abon_list->last->next = abon_node;
		abon_node->prev = abon_list->last;
		abon_list->last = abon_node;
		abon_node->next = NULL;
	}
}

void show(struct Abonent_List abon_list)
{
	struct Abonent_Node* current_node = abon_list.first;
	while(current_node != NULL)
	{
		printf("Name: %s Surname: %s Number: %s\n", 
			current_node->abonent.name, current_node->abonent.surname, current_node->abonent.number);
		current_node = current_node->next;
	}
}

void find(struct Abonent_List abon_list)
{
	char number[12];

	puts("Enter number:");
	scanf("%11s", number);

	struct Abonent_Node* current_node = abon_list.first;
	while(current_node != NULL)
	{
		if (strcmp(number, current_node->abonent.number) == 0)
		{
			printf("Found match: %s %s\n", current_node->abonent.name, current_node->abonent.surname);
			return;
		}
		current_node = current_node->next;
	}
	
	puts("Not found");
}

void free_list(struct Abonent_List* abon_list)
{
	struct Abonent_Node* next_to_free = NULL;
	while(abon_list->last != NULL)
	{
		next_to_free = abon_list->last->prev;
		free(abon_list->last);
		abon_list->last = next_to_free;
	}
	
	abon_list->first = NULL;
	abon_list->last = NULL;
}
