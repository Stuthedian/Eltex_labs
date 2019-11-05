#include <stdlib.h>
#include <stdio.h>
int try(int status, char* function_name, int error_code);
int try_neg(int status, char* function_name);


int try(int status, char* function_name, int error_code)
{
	if(status == error_code)
	{
		perror(function_name);
		exit(EXIT_FAILURE);
	}
	return status;
}

int try_neg(int status, char* function_name)
{
	return try(status, function_name, -1);
}
