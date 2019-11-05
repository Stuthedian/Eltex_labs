#include <string.h>
#include <assert.h>
#include <stdio.h>

#define SERVER 1
#define NEW_USER 'u'
#define NEW_MESSAGE 'm'
#define USERNAME_MAX 20
#define TEXT_MAX 50
#define MAXMSGSZ 1 + sizeof(long) + USERNAME_MAX + TEXT_MAX

typedef struct 
{
	long mtype;
	char mtext[];
} Message;





