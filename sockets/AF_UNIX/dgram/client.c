#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "try.h"

#define BUFSIZE 50

int main()
{
	int dgram_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
	dgram_socket TRY;
	
	struct sockaddr_un socket_addr;
	socket_addr.sun_family = AF_UNIX;
	char sock_path[80] = {0};
	sprintf(sock_path, "/home/work/AF_UNIX/client.%d", getpid());
	strcpy(socket_addr.sun_path, sock_path);

	bind(dgram_socket, (struct sockaddr *)&socket_addr, sizeof socket_addr) TRY;
	
	struct sockaddr_un server_socket_addr;
	server_socket_addr.sun_family = AF_UNIX;
	strcpy(server_socket_addr.sun_path, "/home/work/AF_UNIX/server_sock");
	
	char buf[BUFSIZE] = "Hello from client!";
	sendto(dgram_socket, buf, strlen(buf) + 1, 0, (struct sockaddr*)&server_socket_addr, sizeof server_socket_addr) TRY;	
	
	recvfrom(dgram_socket, buf, BUFSIZE, 0, NULL, NULL) TRY;
	printf("Received message from server: %s\n", buf);

	close(dgram_socket) TRY;
	unlink(sock_path) TRY;
	return 0;
}
