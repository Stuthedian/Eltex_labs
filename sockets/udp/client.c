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
	try_neg(connect(udp_socket, (struct sockaddr *)&socket_addr , sizeof socket_addr), "connect");
	
	char buf[BUFLEN] = "Hello from client!";
	puts("Client: Sending message to server");
	send(udp_socket, buf, BUFLEN, 0);
	puts("Client: Sent message");
	
	recv(udp_socket, &buf, BUFLEN, 0);
	puts("Client: Got message from server:");
	printf("%s\n", buf);
		
	close(udp_socket);
	return 0;
}
