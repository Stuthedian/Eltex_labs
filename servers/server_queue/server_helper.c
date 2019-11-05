#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <mqueue.h>
#include "server_helper.h"
#include "check.h"

void* process_client(void* arg_que_id)
{
	mqd_t que_id = *((mqd_t*)arg_que_id);
	char buf[BUFSIZE] = {0};
	int client_socket = -1;
	while(1)
	{ 
		mq_receive(que_id, (char*)&client_socket, sizeof(int), NULL) CHECK_IS_NEGATIVE_ONE;
		
		sprintf(buf, "Hello from server! My tid is: %ld", syscall(SYS_gettid));
		write(client_socket, buf, strlen(buf) + 1) CHECK_IS_NEGATIVE_ONE;
		read(client_socket, buf, BUFSIZE) CHECK_IS_NEGATIVE_ONE;
		printf("Received message from client: %s\n", buf);
		
		client_socket = -1;
	}
	return NULL;
}
