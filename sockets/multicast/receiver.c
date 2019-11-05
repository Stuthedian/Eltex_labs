#include <netinet/ip.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include "check.h"

#define BUF 50
#define PORT 59123

int main()
{
	int multicast_socket = socket(AF_INET, SOCK_DGRAM, 0);
	multicast_socket CHECK_IS_NEGATIVE_ONE;
	
	struct sockaddr_in client_sockaddr;
	socklen_t client_sockaddr_len = sizeof(client_sockaddr);
	client_sockaddr.sin_family = AF_INET;
	client_sockaddr.sin_port = htons(PORT);
	client_sockaddr.sin_addr.s_addr = INADDR_ANY;
	
	bind(multicast_socket, (struct sockaddr*)&client_sockaddr, sizeof(client_sockaddr)) CHECK_IS_NEGATIVE_ONE;
	
	struct ip_mreqn mgroup;
	inet_pton(AF_INET, "224.0.0.1", &mgroup.imr_multiaddr);
	mgroup.imr_address.s_addr = INADDR_ANY;
	mgroup.imr_ifindex = 0;
	
	setsockopt(multicast_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mgroup, sizeof(mgroup)) CHECK_IS_NEGATIVE_ONE;

	char message[BUF] = {0};
	recvfrom(multicast_socket, message, BUF, 0, 
		(struct sockaddr*)&client_sockaddr, &client_sockaddr_len) CHECK_IS_NEGATIVE_ONE;
	printf("Got message: %s", message);
	
	close(multicast_socket) CHECK_IS_NEGATIVE_ONE;
	return 0;
}
