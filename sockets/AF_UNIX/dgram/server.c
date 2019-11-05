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
	const char* sock_path = "/home/work/AF_UNIX/server_sock";
	if(unlink(sock_path) == -1 && errno != ENOENT)
		TRY_EXIT;
	strcpy(socket_addr.sun_path, sock_path);
	
	bind(dgram_socket, (struct sockaddr *)&socket_addr, sizeof socket_addr) TRY;
		
	struct sockaddr_un client_socket_addr;
	socklen_t client_socket_addr_len = sizeof client_socket_addr;
	char buf[BUFSIZE] = {0};
	recvfrom(dgram_socket, buf, BUFSIZE, 0, (struct sockaddr*)&client_socket_addr, &client_socket_addr_len) TRY;
	printf("Received message from client: %s\n", buf);
	
	strcpy(buf, "Hello from server!");
	sendto(dgram_socket, buf, strlen(buf) + 1, 0, (struct sockaddr*)&client_socket_addr, client_socket_addr_len) TRY;
		
	close(dgram_socket) TRY;
	unlink(sock_path) TRY;
	return 0;
}



