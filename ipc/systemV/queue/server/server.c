#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/msg.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include "Linked_List.h"
#include "message_type.h"

int main()
{
	struct Linked_List client_list;
	client_list.first = NULL;
	client_list.last = NULL;
	Message message;
	
	FILE* msg_que_id = fopen("msg_que_id", "w");
	int msg_que = msgget(1, IPC_CREAT | S_IRWXU);
	fprintf(msg_que_id, "%d", msg_que);
	fclose(msg_que_id);
	

	
	ssize_t a;
	while(1)
	{
		puts("");
		a = msgrcv(msg_que, &message, MAXMSGSZ, 1, 0);
		if(a == -1)
		{
			int errsv = errno;
			puts(strerror(errsv));
		}
		
		printf("%ld %c %ld %s", message.mtype, message.mtext[0], *((long*)(message.mtext+1)),
			message.mtext + 1 + sizeof(long));
		if(message.mtext[0] == NEW_USER)
		{
			long client_pid = *((long*)(message.mtext+1));
			add(&client_list, client_pid);
			struct Linked_List_Node* curr = client_list.first;
			while(curr != NULL)
			{
				message.mtype = curr->num;
				msgsnd(msg_que, &message, MAXMSGSZ, 0);
				curr = curr->next;
			}
		}
		else if(message.mtext[0] == NEW_MESSAGE)
		{
			int n = 1+sizeof(long);
			printf(" %s", message.mtext + n + strlen(message.mtext+n) + 1);
			struct Linked_List_Node* curr = client_list.first;
			while(curr != NULL)
			{
				message.mtype = curr->num;
				msgsnd(msg_que, &message, MAXMSGSZ, 0);
				curr = curr->next;
			}
		}
	}

	return 0;
}
