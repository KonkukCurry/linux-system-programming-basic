#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// argc : # of info that goes to main function
// argv : real info that goes to main, first one is always program execute path
int main(int argc, char** argv)
{
    struct stat statbuf;

    if (argc < 2)
    {
        printf("usage %s filename\n", argv[0]);
        return -1;
    }

    if(stat(argv[1], &statbuf)) // stat(pathname, statbuf)
    {
        printf("stat() fail\n");
        return -1;
    }

    printf("filename %s\n", argv[1]);
    printf("size is %ld\n", statbuf.st_size);

    if(S_ISDIR(statbuf.st_mode))
    {
        printf("it is directory\n");
    }
    else if(S_ISREG(statbuf.st_mode))
    {
        printf("it is regular file\n");
    }
    else
    {
        printf("???\n");
    }
    

    return 0;
}