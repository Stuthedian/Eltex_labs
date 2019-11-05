#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>

int main()
{
    FILE *fp;
    char readbuf[80] = {0};

    umask(0);
    mkfifo("NAMED_PIPE", S_IFIFO|0666);

    fp = fopen("NAMED_PIPE", "r");
    fgets(readbuf, 80, fp);
    printf("Received string: %s\n", readbuf);
    fclose(fp);
    

    return 0;
}
