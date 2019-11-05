#include "try.h"

void print_and_exit(char* filename, int line)
{
	fprintf(stderr, "--%s,  line %d: ", filename, line);
	fflush(stderr);
	perror(NULL);
	exit(EXIT_FAILURE);
}
