#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <string.h>
#include "check.h"

unsigned short checksum(unsigned short* buff, int _16bitword);

int main(int argc, char *argv[])
{
	int icmp_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	icmp_socket CHECK_IS_NEGATIVE_ONE;
	struct sockaddr_in host_socket_addr;
	host_socket_addr.sin_family = AF_INET;
	if(argc != 2)
	{
		puts("Expected one command line parameter");	
		return 0;
	}
	if(inet_pton(AF_INET, argv[1], &(host_socket_addr.sin_addr)) == 0)
	{
		puts("Invalid network address");
		return 0;
	}
	socklen_t host_socket_len = sizeof host_socket_addr;
	
	struct icmphdr icmp_header;
	icmp_header.type = ICMP_ECHO;
	icmp_header.code = 0;
	icmp_header.un.echo.id = 0;
	icmp_header.un.echo.sequence = 0;
	icmp_header.checksum = 0;
	icmp_header.checksum = htons(checksum((unsigned short*)&icmp_header, sizeof(icmp_header)/2));

	sendto(icmp_socket, &icmp_header, sizeof(icmp_header), 0, 
		(struct sockaddr *)&host_socket_addr, host_socket_len) CHECK_IS_NEGATIVE_ONE;
	
	struct iphdr ip_header;
	struct icmphdr received_icmp_header;
	char* str = malloc(sizeof(ip_header) + sizeof(received_icmp_header));
	str CHECK_IS_NULL;
	
	char ip_source_addr[INET_ADDRSTRLEN] = {0};
	while(1)
	{	

		recvfrom(icmp_socket, str, sizeof(ip_header) + sizeof(received_icmp_header), 0, 
			(struct sockaddr *)&host_socket_addr, &host_socket_len) CHECK_IS_NEGATIVE_ONE;
		
		memcpy(&ip_header, str, sizeof(ip_header));
		memcpy(&received_icmp_header, str + sizeof(ip_header), sizeof(received_icmp_header));
		
		inet_ntop(AF_INET, &ip_header.saddr,  ip_source_addr, INET_ADDRSTRLEN) CHECK_IS_NULL;
		if(strcmp(ip_source_addr, argv[1]) == 0)
		{
			if(received_icmp_header.type == ICMP_ECHOREPLY && received_icmp_header.code == 0)
			{
				if(icmp_header.un.echo.id == received_icmp_header.un.echo.id
					&& icmp_header.un.echo.sequence == received_icmp_header.un.echo.sequence)
				{
					printf("Echo reply from %s\n", ip_source_addr);
					icmp_header.un.echo.sequence = htons(ntohs(icmp_header.un.echo.sequence) + 1);
					icmp_header.checksum = 0;
					icmp_header.checksum = htons(checksum((unsigned short*)&icmp_header, sizeof(icmp_header)/2));

					sendto(icmp_socket, &icmp_header, sizeof(icmp_header), 0, 
						(struct sockaddr *)&host_socket_addr, host_socket_len) CHECK_IS_NEGATIVE_ONE;
				}
			}
		}
	}
	free(str);
	close(icmp_socket) CHECK_IS_NEGATIVE_ONE;
	return 0;
}

unsigned short checksum(unsigned short* buff, int _16bitword)
{
        unsigned long sum;
        for(sum=0;_16bitword>0;_16bitword--)
                sum+=htons(*(buff)++);
        sum = ((sum >> 16) + (sum & 0xFFFF));
        sum += (sum>>16);
        return (unsigned short)(~sum);
}
