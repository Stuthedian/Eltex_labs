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
	strcpy(socket_addr.sun_path, "/home/work/AF_UNIX/my_sock");

	connect(stream_socket, (struct sockaddr *)&socket_addr , sizeof socket_addr) TRY;
	
	char buf[BUFSIZE] = {0};
	read(stream_socket, buf, BUFSIZE);
	printf("Received message from server: %s\n", buf);
	strcpy(buf, "Hello from client!");
	write(stream_socket, buf, strlen(buf) + 1);
	
	close(stream_socket) TRY;
	return 0;
}
