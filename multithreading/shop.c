#include "shop.h"

void* go_shopping(void* args)
{
	Arg_struct* arg_struct = args;
	Customer* cust = arg_struct->cust;
	int cust_num = arg_struct->cust_num;
	Shop* shops = arg_struct->shops;
	int size = arg_struct->size;
	
	int rand_value = -1;
		
	while(1)
	{
		if(cust->amount == 0)
		{
			printf("Customer [%d]: i'm done\n", cust_num);
			break;
		}
		//choose a random shop
		rand_value = random() % size;
		
		//check is shop already busy
		if(pthread_mutex_trylock(&shops[rand_value].mutex) == 0)
		{
	
			printf("Customer [%d] in shop [%d]\n", cust_num, rand_value);
			
			//if shop is free subtract amounts
			int diff = abs(cust->amount - shops[rand_value].amount);
			if(diff == 0)
			{
				cust->amount = 0;
				shops[rand_value].amount = 0;
			}
			else
			{
				if(cust->amount > shops[rand_value].amount)
				{
					cust->amount -= shops[rand_value].amount;
					shops[rand_value].amount = 0;
				}
				else
				{
					shops[rand_value].amount -= cust->amount;
					cust->amount = 0;
				}
			}
			printf("Customer's amount after shopping = %d\n", cust->amount);
			pthread_mutex_unlock(&shops[rand_value].mutex);
		}
		
		sleep(1);
	}
}

void* fill_shops(void* args)
{
	Arg_struct* arg_struct = args;
	Shop* shops = arg_struct->shops;
	int size = arg_struct->size;
	
	int rand_value = -1;
	
	while(1)
	{
		rand_value = random() % size;
		if(pthread_mutex_trylock(&shops[rand_value].mutex) == 0)
		{
			printf("Filling shop [%d]\n", rand_value);
			shops[rand_value].amount += 1000;
			pthread_mutex_unlock(&shops[rand_value].mutex);
		}
			
		sleep(1);	
	}
}
