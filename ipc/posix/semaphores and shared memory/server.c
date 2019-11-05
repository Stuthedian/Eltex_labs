#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include "try.h"


#define BUFSIZE 50

int main()
{
	//create shared memory region
	const char* shm_name = "/my_shm";
	int shm_id;
	shm_id = shm_open(shm_name, O_CREAT | O_RDWR, S_IRWXU);
	shm_id TRY;
	ftruncate(shm_id, BUFSIZE) TRY;
	
	char* shm_ptr;
	shm_ptr = mmap(NULL, BUFSIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, shm_id, 0);
	shm_ptr TRY_G(==, MAP_FAILED);
	
	
	//create server semaphore init to 0
	sem_t* server_sem;
	const char* server_sem_name = "/my_serv_sem";
	server_sem = sem_open(server_sem_name, O_CREAT | O_RDWR, S_IRWXU, 0);
	server_sem TRY_G(==, SEM_FAILED);
	
	sem_t* client_sem;
	const char* client_sem_name = "/my_client_sem";
	client_sem = sem_open(client_sem_name, O_CREAT | O_RDWR, S_IRWXU, 0);
	client_sem TRY_G(==, SEM_FAILED);
	
	

	
	//write message to memory
	strcpy(shm_ptr, "Hello from server!");
	
	//increment client's semaphore
	sem_post(client_sem) TRY;
	
	//decrement server's semaphore
	sem_wait(server_sem) TRY;
	//read response from memory
	printf("Received message from client: %s\n", shm_ptr);
	
	//close & unlink semaphore & memory
	sem_close(client_sem) TRY;
	sem_close(server_sem) TRY;
	sem_unlink(server_sem_name) TRY;
	munmap(shm_ptr, BUFSIZE) TRY;
	close(shm_id) TRY;
	shm_unlink(shm_name) TRY;
	return 0;
}

