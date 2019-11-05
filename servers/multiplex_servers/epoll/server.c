#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "../check.h"
#include "../handle_client.h"

#define MINUTE 60*1000

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
	
	
	int events_monitor = epoll_create1(0);
	events_monitor CHECK_IS_NEGATIVE_ONE;

	struct epoll_event read_event;
	read_event.events = EPOLLIN;
	
	read_event.data.fd = tcp_socket;
	epoll_ctl(events_monitor, EPOLL_CTL_ADD, tcp_socket, &read_event) CHECK_IS_NEGATIVE_ONE;
	
	read_event.data.fd = udp_socket;
	epoll_ctl(events_monitor, EPOLL_CTL_ADD, udp_socket, &read_event) CHECK_IS_NEGATIVE_ONE;
	
	struct epoll_event received_event;
	int num_fds = epoll_wait(events_monitor, &received_event, 1, MINUTE);
	if(num_fds == -1)
		print_error_message_and_exit(__FILE__, __LINE__, "poll", errno);
	else if(num_fds == 0)
		puts("Timeout expired");
	else
	{
		if(received_event.data.fd == tcp_socket)
			handle_tcp_client(tcp_socket);
		else 
			handle_udp_client(udp_socket);
	}
	
	close(events_monitor) CHECK_IS_NEGATIVE_ONE;
	close(udp_socket) CHECK_IS_NEGATIVE_ONE;
	close(tcp_socket) CHECK_IS_NEGATIVE_ONE;
	return 0;
}
