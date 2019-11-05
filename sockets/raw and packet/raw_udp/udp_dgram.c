#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "udp_dgram.h"
#include "check.h"

char* udp_dgram_to_char_array(UdpDatagram* udp_dgram)
{
	udp_dgram->length = N*4 + strlen(udp_dgram->data) + 1;
	char* str = malloc(udp_dgram->length);
	str CHECK_IS_NULL;
	
	
	int offset = 0;
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
	
	int offset = 0;
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
