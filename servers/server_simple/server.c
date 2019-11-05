#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "try.h"

#define BUFSIZE 100
void* process_client(void* client_socket_fd);


int main()
{
	int stream_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	stream_socket TRY;

	struct sockaddr_un socket_addr;
	socket_addr.sun_family = AF_UNIX;
	const char* sock_path = "/home/work/servers/server_socket";
	if(unlink(sock_path) == -1 && errno != ENOENT)
		TRY_EXIT;
	strcpy(socket_addr.sun_path, sock_path);
	
	bind(stream_socket, (struct sockaddr *)&socket_addr, sizeof socket_addr) TRY;
	listen(stream_socket, 5) TRY;
	
	pthread_t client_thread;
	pthread_attr_t thread_attr;
	int status;
	status = pthread_attr_init(&thread_attr);
	if(status != 0)
	{
		fprintf(stderr, "pthread_attr_init: %s\n", strerror(status));
		exit(EXIT_FAILURE);
	}
	
	status = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	if(status != 0)
	{
		fprintf(stderr, "pthread_attr_setdetachstate: %s\n", strerror(status));
		exit(EXIT_FAILURE);
	}
	struct sockaddr_un client_socket_addr;
	socklen_t client_socket_len = sizeof client_socket_addr;
	int client_socket;
	while(1)
	{
		client_socket = accept(stream_socket, (struct sockaddr *)&client_socket_addr, &client_socket_len);
		client_socket TRY;
		
		status = pthread_create(&client_thread, &thread_attr, process_client, &client_socket);
		if(status != 0)
		{
			fprintf(stderr, "pthread_create: %s\n", strerror(status));
			exit(EXIT_FAILURE);
		}
	}

	
	status = pthread_attr_destroy(&thread_attr);
	if(status != 0)
	{
		fprintf(stderr, "pthread_attr_destroy: %s\n", strerror(status));
		exit(EXIT_FAILURE);
	}
	close(stream_socket) TRY;
	unlink(sock_path) TRY;
	return 0;
}

void* process_client(void* client_socket_fd)
{
	int client_socket = *((int*)client_socket_fd);

	char buf[BUFSIZE] = "Hello from server!";
	write(client_socket, buf, strlen(buf) + 1);
	read(client_socket, buf, BUFSIZE);
	printf("Received message from client: %s\n", buf);	
	
	return NULL;
}


