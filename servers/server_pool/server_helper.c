#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "server_helper.h"
#include "check.h"

void* process_client(void* arg_pool_server)
{
	pool_server* server =  arg_pool_server;
	char buf[BUFSIZE] = {0};
	while(1)
	{ 
		sem_wait(server->can_read) CHECK_IS_NEGATIVE_ONE;
		
		sprintf(buf, "Hello from server! My tid is: %ld", syscall(SYS_gettid));
		write(server->client_socket, buf, strlen(buf) + 1) CHECK_IS_NEGATIVE_ONE;
		read(server->client_socket, buf, BUFSIZE) CHECK_IS_NEGATIVE_ONE;
		printf("Received message from client: %s\n", buf);
		
		server->client_socket = -1;
		sem_post(server->can_write) CHECK_IS_NEGATIVE_ONE;
	}
	return NULL;
}
