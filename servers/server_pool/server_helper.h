#include <pthread.h>
#include <semaphore.h>
#define BUFSIZE 100
struct pool_server_ 
{
	pthread_t server_thread;
	int client_socket;
	sem_t* can_read;
	sem_t* can_write;
};
typedef struct pool_server_ pool_server;

void* process_client(void* arg_pool_server);
