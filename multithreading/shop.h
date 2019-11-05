#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
//#include <unistd.h>
//#include <string.h>


typedef struct
{
	int amount;
	pthread_mutex_t mutex;
} Shop;

typedef struct
{
	int amount;
	pthread_t thread;
} Customer;

typedef struct
{
	Customer* cust;
	int cust_num;
	Shop* shops;
	int size;
} Arg_struct;

void* go_shopping(void* args);
void* fill_shops(void* args);

