#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "../check.h"
#include "../handle_client.h"

int main()
{
	int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
	tcp_socket CHECK_IS_NEGATIVE_ONE;
	
	int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
	udp_socket CHECK_IS_NEGATIVE_ONE;
	
	struct sockaddr_in socket_addr;
	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port = htons(12345);
	inet_pton(AF_INET, "127.1.2.3", &(socket_addr.sin_addr));
	
	bind(tcp_socket, (struct sockaddr *)&socket_addr, sizeof socket_addr) CHECK_IS_NEGATIVE_ONE;
	listen(tcp_socket, 5) CHECK_IS_NEGATIVE_ONE;
	
	bind(udp_socket, (struct sockaddr *)&socket_addr, sizeof socket_addr) CHECK_IS_NEGATIVE_ONE;
	
	struct timeval timeout;
	timeout.tv_sec = 60;
	timeout.tv_usec = 0;
	
	fd_set read_fds;
	FD_ZERO(&read_fds);
	FD_SET(tcp_socket, &read_fds);
	FD_SET(udp_socket, &read_fds);
	int max_fd = tcp_socket > udp_socket ? tcp_socket : udp_socket;
	
	int num_fds = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);
	if(num_fds == -1)
		print_error_message_and_exit(__FILE__, __LINE__, "select", errno);
	else if(num_fds == 0)
		puts("Timeout expired");
	else
	{
		if(FD_ISSET(tcp_socket, &read_fds))
			handle_tcp_client(tcp_socket);
		else 
			handle_udp_client(udp_socket);
	}
	
	
	close(udp_socket) CHECK_IS_NEGATIVE_ONE;
	close(tcp_socket) CHECK_IS_NEGATIVE_ONE;
	return 0;
}
