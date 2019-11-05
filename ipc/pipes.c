#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define IN 0
#define OUT 1

int main()
{
    int     _pipe[2];
    pid_t   childpid;
    char    string[] = "Hello, world!\n";
    char    readbuffer[80];

    pipe(_pipe);

    if((childpid = fork()) == -1)
    {
        perror("fork");
        return 1;
    }

    if(childpid == 0)
    {
    	
       close(_pipe[OUT]);
       dup2(_pipe[IN], IN);
       execlp("wc", "wc", "-m", NULL);
	}
	else
	{	
		
		close(_pipe[IN]);
        write(_pipe[OUT], string, (strlen(string)+1));
    }
    
    return 0;
}
