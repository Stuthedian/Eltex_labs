#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main()
{
    FILE *pipe_fp;
    char    string[] = "Hello, world!";
 
 	if (( pipe_fp = popen("wc -m", "w")) == NULL)
    {
            perror("popen");
            return 1;
    }
    
    fputs(string, pipe_fp);
	
    pclose(pipe_fp);
    return 0;
}
