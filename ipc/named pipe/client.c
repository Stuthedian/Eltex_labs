#include <stdio.h>
#include <stdlib.h>

int main()
{
        FILE *fp;

        if((fp = fopen("NAMED_PIPE", "w")) == NULL) 
        {
            perror("fopen");
            return 1;
        }

        fputs("Hello from client!", fp);

        fclose(fp);
        return 0;
}
