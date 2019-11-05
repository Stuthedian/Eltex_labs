#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include "udp_dgram.h"
#include "check.h"

#define UDP 17

char* udp_dgram_to_char_array(UdpDatagram* udp_dgram)
{
	udp_dgram->length = N*4 + strlen(udp_dgram->data) + 1;
	char* str = malloc(udp_dgram->length);
	str CHECK_IS_NULL;
	
	
	unsigned int offset = 0;
	memcpy(str + offset, (char*)&udp_dgram->src_port, N);
	offset += N;
	
	memcpy(str + offset, (char*)&udp_dgram->dest_port, N);
	offset += N;
	
	udp_dgram->length = htons(udp_dgram->length);
	memcpy(str + offset, (char*)&udp_dgram->length, N);
	udp_dgram->length = ntohs(udp_dgram->length);
	offset += N;
	
	memcpy(str + offset, (char*)&udp_dgram->checksum, N);
	offset += N;
	
	strcpy(str + offset, udp_dgram->data);
	
	return str;
}

UdpDatagram* char_array_to_udp_dgram(char* message)
{
	UdpDatagram* udp_dgram = malloc(sizeof (UdpDatagram));
	udp_dgram CHECK_IS_NULL;
	
	const int ip_header_len = 20;
	char* udp_starts = message + ip_header_len;
	
	unsigned int offset = 0;
	memcpy((char*)&udp_dgram->src_port, udp_starts + offset, N);
	udp_dgram->src_port = ntohs(udp_dgram->src_port);
	offset += N;
	
	memcpy((char*)&udp_dgram->dest_port, udp_starts + offset,  N);
	udp_dgram->dest_port = ntohs(udp_dgram->dest_port);
	offset += N;
	
	memcpy((char*)&udp_dgram->length, udp_starts + offset,  N);
	udp_dgram->length = ntohs(udp_dgram->length);
	offset += N;
	
	memcpy((char*)&udp_dgram->checksum, udp_starts + offset,  N);
	udp_dgram->checksum = ntohs(udp_dgram->checksum);
	offset += N;
	
	udp_dgram->data = udp_starts + offset;
	
	return udp_dgram;
}

char* make_packet(char* data, int src_port, char* src_ip, char* src_mac, int dest_port, char* dest_ip, char* dest_mac, int* n)
{
	struct udphdr udp_header;
	udp_header.source = htons(src_port);
	udp_header.dest = htons(dest_port);
	udp_header.len = htons(sizeof(udp_header) + strlen(data) + 1);
	udp_header.check = 0;
	
	struct iphdr ip_header;
	int ip_total_len = sizeof(ip_header) + ntohs(udp_header.len);
	ip_header.version = 4;
	ip_header.ihl = 5;
	ip_header.tos = 0;
	ip_header.id = 0;
	ip_header.frag_off = 0;
	ip_header.ttl = 64;
	ip_header.protocol = UDP;
	ip_header.check = 0;
	ip_header.tot_len = htons(ip_total_len);
	inet_pton(AF_INET, src_ip, &(ip_header.saddr));
	inet_pton(AF_INET, dest_ip, &(ip_header.daddr));
	
	ip_header.check = htons(checksum((unsigned short*)&ip_header, sizeof(ip_header)/2));
	
	struct	ethhdr ethernet_header;
	string_to_mac_array(dest_mac, ethernet_header.h_dest);
	string_to_mac_array(src_mac, ethernet_header.h_source);
	ethernet_header.h_proto = htons(ETHERTYPE_IP);
	
	int packet_size = sizeof(ethernet_header) + ntohs(ip_header.tot_len);
	char* packet = malloc(packet_size);
	packet CHECK_IS_NULL;
	
	int offset = 0;
	memcpy(packet + offset, &ethernet_header, sizeof(ethernet_header));
	offset += sizeof(ethernet_header);
	
	memcpy(packet + offset, &ip_header, sizeof(ip_header));
	offset += sizeof(ip_header);
	
	memcpy(packet + offset, &udp_header, sizeof(udp_header));
	offset += sizeof(udp_header);
	
	strcpy(packet + offset, data);
	
	*n = packet_size;
	return packet;
}

void string_to_mac_array(char* str, unsigned char* mac_arr)
{
	for (int i = 0; i < 6; i++)
	{
		mac_arr[i] = strtoul(str, NULL, 16);
		str += 3;
	}
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
