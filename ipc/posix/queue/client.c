#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include "try.h"

#define BUFSIZE 50

int main()
{
	const char* que_name = "/my_queue";
	mqd_t que_id;
	que_id = mq_open(que_name, O_RDWR, 0, NULL);
	que_id TRY;
	
	char buf[BUFSIZE] = "Hello from client!";
	mq_send(que_id, buf, strlen(buf) + 1, 0) TRY;
	
	mq_receive(que_id, buf, BUFSIZE, NULL) TRY;
	printf("Received message from server: %s\n", buf);
	
	mq_close(que_id) TRY;
	return 0;
}
