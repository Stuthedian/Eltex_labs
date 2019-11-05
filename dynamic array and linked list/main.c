#include <stdio.h>
#include <stdlib.h>
#include "abonent.h"


int main()
{
	struct Abonent_List abon_list;
	abon_list.first = NULL;
	abon_list.last = NULL;	
	
	int flag = 1;
	char choice;
	puts("1 - Add abonent\n2 - Show all abonents\n3 - Find by number\n4 - Exit");

	while (flag)
	{
		scanf("%c", &choice);
		fseek(stdin, 0, SEEK_END);

		switch (choice)
		{
		case '1': add(&abon_list); break;
		case '2': show(abon_list); break;
		case '3': find(abon_list); break;
		case '4': flag = 0; break;
		default:
			puts("1 - Add abonent\n2 - Show all abonents\n3 - Find by number\n4 - Exit");
			break;
		}
	}
	
	free_list(&abon_list);
    return 0;
}

