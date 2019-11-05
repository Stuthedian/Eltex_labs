#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "server_helper.h"
#include "check.h"

#define POOLSIZE 5

int main()
{
	pthread_attr_t thread_attr;
	INTERPRET_AS_ERRNUM_IF_NOT_ZERO(pthread_attr_init(&thread_attr));
	INTERPRET_AS_ERRNUM_IF_NOT_ZERO(pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED));
	
	pool_server pool[POOLSIZE];
	sem_t read_sems[POOLSIZE];
	sem_t write_sems[POOLSIZE];
	
	for(int i = 0; i < POOLSIZE; i++)
	{
		sem_init(&read_sems[i], 0, 0) CHECK_IS_NEGATIVE_ONE;
		sem_init(&write_sems[i], 0, 1) CHECK_IS_NEGATIVE_ONE;
		pool[i].can_read = &read_sems[i];
		pool[i].can_write = &write_sems[i];
		pool[i].client_socket = -1;
		
		INTERPRET_AS_ERRNUM_IF_NOT_ZERO(pthread_create(&pool[i].server_thread, &thread_attr, process_client, &pool[i]));
	}
	INTERPRET_AS_ERRNUM_IF_NOT_ZERO(pthread_attr_destroy(&thread_attr));
	
	int stream_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	stream_socket CHECK_IS_NEGATIVE_ONE;
	
	struct sockaddr_un socket_addr;
	socket_addr.sun_family = AF_UNIX;
	const char* sock_path = "/home/work/servers/server_socket";
	if(unlink(sock_path) == -1 && errno != ENOENT)
		print_error_message_and_exit(__FILE__,  __LINE__, "unlink", errno);
	strcpy(socket_addr.sun_path, sock_path);

	bind(stream_socket, (struct sockaddr *)&socket_addr, sizeof socket_addr) CHECK_IS_NEGATIVE_ONE;
	listen(stream_socket, 5) CHECK_IS_NEGATIVE_ONE;

	struct sockaddr_un client_socket_addr;
	socklen_t client_socket_len = sizeof client_socket_addr;
	int client_socket;
	
	while(1)
	{
		client_socket = accept(stream_socket, (struct sockaddr *)&client_socket_addr, &client_socket_len);
		client_socket CHECK_IS_NEGATIVE_ONE;
		
		for(int i = 0; i < POOLSIZE; i++)
		{
			int status = sem_trywait(pool[i].can_write);

			if(status == -1)
			{
				if(errno != EAGAIN)
					print_error_message_and_exit(__FILE__,  __LINE__, "sem_trywait", errno);
			}
			else
			{
				pool[i].client_socket = client_socket;
				sem_post(pool[i].can_read) CHECK_IS_NEGATIVE_ONE;
				break;
			}
		}
	}

	close(stream_socket) CHECK_IS_NEGATIVE_ONE;
	unlink(sock_path) CHECK_IS_NEGATIVE_ONE;
	return 0;
}




