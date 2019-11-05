#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <mqueue.h>
#include "server_helper.h"
#include "check.h"

#define POOLSIZE 5

int main()
{

	const char* que_name = "/my_queue";
	struct mq_attr que_attr;
	que_attr.mq_msgsize = sizeof(int);
	que_attr.mq_maxmsg = POOLSIZE * 2;
	
	mqd_t que_id;
	que_id = mq_open(que_name, O_CREAT | O_RDWR, S_IRWXU, &que_attr);
	que_id CHECK_IS_NEGATIVE_ONE;
	
	pthread_attr_t thread_attr;
	INTERPRET_AS_ERRNUM_IF_NOT_ZERO(pthread_attr_init(&thread_attr));
	INTERPRET_AS_ERRNUM_IF_NOT_ZERO(pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED));
	
	pthread_t pool[POOLSIZE];
	
	for(int i = 0; i < POOLSIZE; i++)
	{
		INTERPRET_AS_ERRNUM_IF_NOT_ZERO(pthread_create(&pool[i], &thread_attr, process_client, &que_id));
	}
	INTERPRET_AS_ERRNUM_IF_NOT_ZERO(pthread_attr_destroy(&thread_attr));
	
	int stream_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	stream_socket CHECK_IS_NEGATIVE_ONE;
	
	struct sockaddr_un socket_addr;
	socket_addr.sun_family = AF_UNIX;
	const char* sock_path = "/home/work/servers/server_socket";
	if(unlink(sock_path) == -1 && errno != ENOENT)
		print_error_message_and_exit(__FILE__,  __LINE__, "unlink", errno);
	strcpy(socket_addr.sun_path, sock_path);

	bind(stream_socket, (struct sockaddr *)&socket_addr, sizeof socket_addr) CHECK_IS_NEGATIVE_ONE;
	listen(stream_socket, 5) CHECK_IS_NEGATIVE_ONE;

	struct sockaddr_un client_socket_addr;
	socklen_t client_socket_len = sizeof client_socket_addr;

	int client_socket;
	while(1)
	{
		client_socket = accept(stream_socket, (struct sockaddr *)&client_socket_addr, &client_socket_len);
		client_socket CHECK_IS_NEGATIVE_ONE;
		mq_send(que_id, (char*)&client_socket, sizeof(int), 0) CHECK_IS_NEGATIVE_ONE;
	}

	close(stream_socket) CHECK_IS_NEGATIVE_ONE;
	unlink(sock_path) CHECK_IS_NEGATIVE_ONE;
	mq_close(que_id) CHECK_IS_NEGATIVE_ONE;
	mq_unlink(que_name) CHECK_IS_NEGATIVE_ONE;
	return 0;
}




