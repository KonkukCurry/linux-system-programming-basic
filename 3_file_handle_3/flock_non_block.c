#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> // for open function
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/file.h> // for flock
#include <errno.h>


static void print_usage(const char* prgname)
{
    printf("usage : %s (ex|sh)\n", prgname);
    exit(-1);
}

int main(int argc, char** argv)
{
    int fd;
    int ops; // option for flock()

    if(argc < 2)
    {
        print_usage(argv[0]);
        // if argument # is lower than 2
        // it's an error
    }

    if(!(strcmp(argv[1], "sh")))
    {
        ops = LOCK_SH;
    }
    else if(!(strcmp(argv[1], "ex")))
    {
        ops = LOCK_EX;
    }
    else
    {
        print_usage(argv[0]);
    }

    fd = open("lockfile", O_RDWR|O_CREAT, 0644);
    if(fd < 0)
    {
        printf("open() fail");
        return -1;
    }

    // flock
    // return suc 0 fail -1
    printf("trying to get lock...\n");
    if(flock(fd, ops | LOCK_NB) == -1) // GIVE NON BLOCK OPTION
    {
        printf("fail to get lock...\n");
        printf("flock(ops, %d), errno = %d(%s)\n", ops, errno, strerror(errno)); // give error number
        goto out; // gather exception
    }
    printf("grab the lock!\n");
    getc(stdin);
    if(flock(fd, LOCK_UN) == -1)
    {
        printf("fail to dump lock...\n");
        goto out; // gather exception
    }
    printf("dump the lock...\n");

    close(fd);
    return 0;

out : 
    close(fd);
    return -1;
}