#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "try.h"

int main()
{
	int tcp_socket = try_neg(socket(AF_INET, SOCK_STREAM, 0), "socket");
	
	struct sockaddr_in socket_addr;
	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port = 0;
	socket_addr.sin_port |= 44;
	socket_addr.sin_port <<= 8;
	socket_addr.sin_port |= 44;//4444
	
	inet_pton(AF_INET, "10.0.2.15", &(socket_addr.sin_addr));
	try_neg(connect(tcp_socket, (struct sockaddr *)&socket_addr , sizeof socket_addr), "connect");
	
	//puts("Connected!");
	char buf[50] = {0};
	char* str = "Hello from client!";
	read(tcp_socket, buf, 50);
	printf("%s\n", buf);
	write(tcp_socket, str, strlen(str));
	
	close(tcp_socket);
	return 0;
}
