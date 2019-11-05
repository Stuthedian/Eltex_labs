#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "check.h"
#include "udp_dgram.h"

#define BUFLEN 50
#define SERVERPORT 12345
#define CLIENTPORT 54321

int main()
{
	int udp_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	udp_socket CHECK_IS_NEGATIVE_ONE;
	
	struct sockaddr_in server_socket_addr;
	server_socket_addr.sin_family = AF_INET;
	server_socket_addr.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, "127.1.2.3", &(server_socket_addr.sin_addr));
	uint server_socket_len = sizeof server_socket_addr;
	
	char buf[BUFLEN] = "Hello from client!";
	
	UdpDatagram udp_dgram;
	udp_dgram.src_port = htons(CLIENTPORT);
	udp_dgram.dest_port = htons(SERVERPORT);
	udp_dgram.length = 0;
	udp_dgram.data = buf;
	udp_dgram.checksum = 0;
	
	char* message = udp_dgram_to_char_array(&udp_dgram);
	
	sendto(udp_socket, message, udp_dgram.length, 0, 
		(struct sockaddr *)&server_socket_addr, server_socket_len) CHECK_IS_NEGATIVE_ONE;

	UdpDatagram* received_udp_dgram;
	while(1)
	{	
		recvfrom(udp_socket, buf, BUFLEN, 0, 
			(struct sockaddr *)&server_socket_addr, &server_socket_len) CHECK_IS_NEGATIVE_ONE;
		received_udp_dgram = char_array_to_udp_dgram(buf);
		if(received_udp_dgram->dest_port == CLIENTPORT)
		{
			printf("Got message from server: %s\n", received_udp_dgram->data);			
			free(received_udp_dgram);
			break;
		}
		free(received_udp_dgram);
	}
		
		
	close(udp_socket) CHECK_IS_NEGATIVE_ONE;
	free(message);
	return 0;
}
