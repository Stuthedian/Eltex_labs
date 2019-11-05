#include <netinet/ip.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include "check.h"

#define PORT 59123

int main()
{
	int multicast_socket = socket(AF_INET, SOCK_DGRAM, 0);
	multicast_socket CHECK_IS_NEGATIVE_ONE;
	
	struct sockaddr_in receiver_sockaddr;
	receiver_sockaddr.sin_family = AF_INET;
	receiver_sockaddr.sin_port = htons(PORT);
	inet_pton(AF_INET, "224.0.0.1", &receiver_sockaddr.sin_addr.s_addr);
	
	char* message = "Multicast message!";
	sendto(multicast_socket, message, strlen(message) + 1, 0, 
		(struct sockaddr*)&receiver_sockaddr, sizeof(receiver_sockaddr)) CHECK_IS_NEGATIVE_ONE;
	
	close(multicast_socket) CHECK_IS_NEGATIVE_ONE;
	return 0;
}
