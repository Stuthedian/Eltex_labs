#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include "Linked_List.h"
#include "message_type.h"

union semun 
{
	int val;
	struct semid_ds * buf;
	unsigned short * array;
#if defined(__linux__)
	struct seminfo * __buf;
#endif
};

int main()
{
	struct Linked_List client_list;
	client_list.first = NULL;
	client_list.last = NULL;
	Message* message;
	union semun init_sem;
	init_sem.val = 0;
	
	FILE* id_file = fopen("id_file", "w");
	int shared_memory_id = shmget(1, sizeof (Message), IPC_CREAT | S_IRWXU);
	int semaphore_id = semget(1, 1, IPC_CREAT | S_IRWXU);
	fprintf(id_file, "%d %d", shared_memory_id, semaphore_id);
	fclose(id_file);
	
	message = shmat(shared_memory_id, NULL, 0);
	
	semctl(semaphore_id, 0, SETVAL, init_sem);
	
	
		
	int a;
	while(1)
	{
		a = decrement_sem(semaphore_id);//wait for client
		if(a == -1)
		{
			perror("semop");
		}
			
		if(message->type == NEW_USER)
			add(&client_list, message->sem_id);
		else if(message->type == NEW_MESSAGE)
			printf(" %s", message->text);
		
		struct Linked_List_Node* curr = client_list.first;
		while(curr != NULL)
		{
			//notify clients one after another that they got new message
			//increase their semaphores to allow them modify shared memory
			increment_sem(curr->num);
			curr = curr->next;
		}
	}

	return 0;
}
