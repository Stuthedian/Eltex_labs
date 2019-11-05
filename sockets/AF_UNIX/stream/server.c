#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "try.h"

#define BUFSIZE 50

int main()
{
	int stream_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	stream_socket TRY;

	struct sockaddr_un socket_addr;
	socket_addr.sun_family = AF_UNIX;
	const char* sock_path = "/home/work/AF_UNIX/my_sock";
	if(unlink(sock_path) == -1 && errno != ENOENT)
		TRY_EXIT;
	strcpy(socket_addr.sun_path, sock_path);
	
	bind(stream_socket, (struct sockaddr *)&socket_addr, sizeof socket_addr) TRY;
	listen(stream_socket, 5) TRY;
	
	struct sockaddr_un client_socket_addr;
	socklen_t client_socket_len = sizeof client_socket_addr;
	int client_socket = accept(stream_socket, (struct sockaddr *)&client_socket_addr, &client_socket_len);
	client_socket TRY;
		
	char buf[BUFSIZE] = "Hello from server!";
	write(client_socket, buf, strlen(buf) + 1);
	read(client_socket, buf, BUFSIZE);
	printf("Received message from client: %s\n", buf);
	
	close(stream_socket) TRY;
	unlink(sock_path) TRY;
	return 0;
}



