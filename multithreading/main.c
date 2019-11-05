#include "shop.h"
#define N 5
#define NC 3

int main()
{
	Shop shops[N];
	Customer customers[NC];
	Arg_struct arg_struct[NC];
	pthread_t fill_thread;
	
	srandom(time(NULL));
	
	long int random(void);

	for (int i = 0; i < N; i += 1)
	{
		pthread_mutex_init(&shops[i].mutex, NULL);
		shops[i].amount = (random() % 201) + 900;
		printf("Shop [%d] has amount = %d\n", i, shops[i].amount);
	}
	
	for (int i = 0; i < NC; i += 1)
	{
		customers[i].amount = (random() % 201) + 3000;
		arg_struct[i].cust = &customers[i];
		arg_struct[i].cust_num = i;
		arg_struct[i].shops = shops;
		arg_struct[i].size = N;		
		pthread_create(&customers[i].thread, NULL, go_shopping, &arg_struct[i]);
	}
	pthread_create(&fill_thread, NULL, fill_shops, &arg_struct[0]);
	
	for (int i = 0; i < NC; i++) 
	{
        pthread_join(customers[i].thread, NULL);
    }
	
	for (int i = 0; i < N; i += 1)
		pthread_mutex_destroy(&shops[i].mutex);
	return 0;
}
