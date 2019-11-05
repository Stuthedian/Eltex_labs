#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "check.h"

#define BUFSIZE 100

int main()
{
	int stream_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	stream_socket CHECK_IS_NEGATIVE_ONE;
	
	struct sockaddr_un socket_addr; 
	socket_addr.sun_family = AF_UNIX;
	strcpy(socket_addr.sun_path, "/home/work/servers/server_socket");

	connect(stream_socket, (struct sockaddr *)&socket_addr , sizeof socket_addr) CHECK_IS_NEGATIVE_ONE;
	
	char buf[BUFSIZE] = {0};
	read(stream_socket, buf, BUFSIZE) CHECK_IS_NEGATIVE_ONE;
	printf("Received message from server: %s\n", buf);
	sprintf(buf, "Hello from client! My pid is: %d", getpid());
	write(stream_socket, buf, strlen(buf) + 1) CHECK_IS_NEGATIVE_ONE;
	
	close(stream_socket) CHECK_IS_NEGATIVE_ONE;
	return 0;
}
