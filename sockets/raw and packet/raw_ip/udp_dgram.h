struct UdpDatagram_
{
	uint16_t src_port;
	uint16_t dest_port;
	uint16_t length;
	uint16_t checksum;
	char* data;
};
typedef struct UdpDatagram_ UdpDatagram;

#define N sizeof(uint16_t)

char* udp_dgram_to_char_array(UdpDatagram* udp_dgram);

UdpDatagram* char_array_to_udp_dgram(char* message);
