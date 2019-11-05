#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "try.h"

#define BUFLEN 50

int main()
{
	int udp_socket = try_neg(socket(AF_INET, SOCK_DGRAM, 0), "socket");

	struct sockaddr_in socket_addr;
	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port = htons(4444);
	inet_pton(AF_INET, "10.0.2.15", &(socket_addr.sin_addr));
	
	try_neg(bind(udp_socket, (struct sockaddr *)&socket_addr , sizeof socket_addr), "bind");
			
	char buf[BUFLEN] = {0};
	struct sockaddr_in client_socket_addr;
	uint client_socket_len = sizeof client_socket_addr;
	recvfrom(udp_socket, buf, BUFLEN, 0, (struct sockaddr *)&client_socket_addr, &client_socket_len);
	puts("Server: Got message from client:");
	printf("%s\n", buf);
	puts("Server: Sending response...");
	strcpy(buf, "Hello from server!");
	sendto(udp_socket, buf, BUFLEN, 0, (struct sockaddr *)&client_socket_addr, client_socket_len);
		
	close(udp_socket);
	return 0;
}



