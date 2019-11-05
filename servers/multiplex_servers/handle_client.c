#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include "handle_client.h"
#include "check.h"

#define BUFSIZE 50

void handle_tcp_client(int tcp_socket)
{
	puts("Handling tcp client...");
	struct sockaddr_in client_socket_addr;
	socklen_t client_socket_len = sizeof client_socket_addr;
	int client_socket = accept(tcp_socket, (struct sockaddr *)&client_socket_addr, &client_socket_len);
	client_socket CHECK_IS_NEGATIVE_ONE;
		
	char buf[BUFSIZE] = {0};
	read(client_socket, buf, 50) CHECK_IS_NEGATIVE_ONE;
	printf("Got message from client: %s\n", buf);
	
	strcpy(buf, "Hello from server!");
	write(client_socket, buf, strlen(buf) + 1) CHECK_IS_NEGATIVE_ONE;
}

void handle_udp_client(int udp_socket)
{
	puts("Handling udp client...");
	struct sockaddr_in client_socket_addr;
	socklen_t client_socket_len = sizeof client_socket_addr;
	
	char buf[BUFSIZE] = {0};
	recvfrom(udp_socket, buf, BUFSIZE, 0, 
		(struct sockaddr *)&client_socket_addr, &client_socket_len) CHECK_IS_NEGATIVE_ONE;
	printf("Got message from client: %s\n", buf);
	
	strcpy(buf, "Hello from server!");
	sendto(udp_socket, buf, strlen(buf) + 1, 0, 
		(struct sockaddr *)&client_socket_addr, client_socket_len) CHECK_IS_NEGATIVE_ONE;
}
