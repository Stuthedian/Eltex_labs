#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>

#define SERVER 1
#define NEW_USER 'u'
#define NEW_MESSAGE 'm'
#define USERNAME_MAX 20
#define TEXT_MAX 50

typedef struct 
{
	char type; // NEW_USER  || NEW_MESSAGE
	int sem_id;
	char username[USERNAME_MAX];
	char text[TEXT_MAX];
} Message;

int increment_sem(int semaphore_id);
int decrement_sem(int semaphore_id);
static int update_sem(int semaphore_id, int value);



