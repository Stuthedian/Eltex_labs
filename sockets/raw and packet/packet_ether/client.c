#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "check.h"
#include "udp_dgram.h"

#define UDP 17
#define BUFLEN 100
#define CLIENTPORT 54321
#define CLIENTIP "192.168.0.101"
#define CLIENTMAC "08:00:27:0D:76:28"
#define SERVERPORT 12345
#define SERVERIP "192.168.0.103"
#define SERVERMAC "08:00:27:0C:D9:B1"



int main()
{
	int packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	packet_socket CHECK_IS_NEGATIVE_ONE;
	
	struct sockaddr_ll server_socket_addr;
	socklen_t server_socket_len = sizeof server_socket_addr;
	memset(&server_socket_addr, 0, server_socket_len);
	
	server_socket_addr.sll_family = AF_PACKET;
	server_socket_addr.sll_ifindex = if_nametoindex("enp0s3");
	server_socket_addr.sll_ifindex CHECK_IS_ZERO;
	server_socket_addr.sll_halen = 6;
	string_to_mac_array(CLIENTMAC, server_socket_addr.sll_addr);
		
	
	char buf[BUFLEN] = "Hello from client!";
	int n = -1;
	char* packet = make_packet(buf, CLIENTPORT, CLIENTIP, CLIENTMAC, SERVERPORT, SERVERIP, SERVERMAC, &n);
	sendto(packet_socket, packet, n, 0, 
		(struct sockaddr *)&server_socket_addr, server_socket_len) CHECK_IS_NEGATIVE_ONE;
	free(packet);
	
	UdpDatagram* received_udp_dgram;
	while(1)
	{	
		recvfrom(packet_socket, buf, BUFLEN, 0, 
			(struct sockaddr *)&server_socket_addr, &server_socket_len) CHECK_IS_NEGATIVE_ONE;
			
		received_udp_dgram = char_array_to_udp_dgram(buf + sizeof(struct ethhdr));
		if(received_udp_dgram->dest_port == CLIENTPORT)
		{
			printf("Got message from server: %s\n", received_udp_dgram->data);			
			free(received_udp_dgram);
			break;
		}
		free(received_udp_dgram);
	}
		
	close(packet_socket) CHECK_IS_NEGATIVE_ONE;
	return 0;
}
