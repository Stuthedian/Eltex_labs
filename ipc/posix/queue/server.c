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
	struct mq_attr que_attr;
	que_attr.mq_msgsize = BUFSIZE;
	que_attr.mq_maxmsg = 5;
	
	mqd_t que_id;
	que_id = mq_open(que_name, O_CREAT | O_RDWR, S_IRWXU, &que_attr);
	que_id TRY;
	
	char buf[BUFSIZE] = {0};
	mq_receive(que_id, buf, BUFSIZE+50, NULL) TRY;
	printf("Received message from client: %s\n", buf);
	
	strcpy(buf, "Hello from server!");
	mq_send(que_id, buf, strlen(buf) + 1, 0) TRY;
	
	mq_close(que_id) TRY;
	mq_unlink(que_name) TRY;
	return 0;
}

