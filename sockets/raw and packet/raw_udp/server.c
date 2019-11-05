#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "check.h"

#define BUFLEN 50

int main()
{
	int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
	udp_socket CHECK_IS_NEGATIVE_ONE;

	struct sockaddr_in socket_addr;
	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port = htons(12345);
	inet_pton(AF_INET, "127.1.2.3", &(socket_addr.sin_addr));
	
	bind(udp_socket, (struct sockaddr *)&socket_addr , sizeof socket_addr) CHECK_IS_NEGATIVE_ONE;
			
	char buf[BUFLEN] = {0};
	
	struct sockaddr_in client_socket_addr;
	uint client_socket_len = sizeof client_socket_addr;
	recvfrom(udp_socket, buf, BUFLEN, 0, 
		(struct sockaddr *)&client_socket_addr, &client_socket_len) CHECK_IS_NEGATIVE_ONE;
	printf("Got message from client: %s\n", buf);
	
	strcpy(buf, "Hello from server!");
	sendto(udp_socket, buf, strlen(buf) + 1, 0, 
		(struct sockaddr *)&client_socket_addr, client_socket_len) CHECK_IS_NEGATIVE_ONE;
		
	close(udp_socket) CHECK_IS_NEGATIVE_ONE;
	return 0;
}



