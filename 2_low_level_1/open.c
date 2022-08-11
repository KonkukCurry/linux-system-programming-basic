#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> // for open function
#include <sys/stat.h>
#include <fcntl.h>

// append
static int append_file(void)
{
    int fd = open("datafile", O_WRONLY | O_APPEND);
    // not for creating so we don't need mode integer
    // The return value of open() is a file descriptor

    if(fd == -1)
    {
        printf("open() error\n");
        return -1;
    }

    // file input fuction, almost same as printf
    dprintf(fd, "Hello Again My Bro! %d \n", 970306);

    close(fd);
    return 0;
}

// similar to write_file but if I have same named file, then erase it
static int trunc_file(void)
{
    int fd = open("datafile", O_WRONLY | O_CREAT | O_TRUNC, 0600); // 6(4+2) rw, 4 r, 4 r
    //The return value of open() is a file descriptor

    if(fd == -1)
    {
        printf("open() error\n");
        return -1;
    }

    // file input fuction, almost same as printf
    dprintf(fd, "Hello TRUNCATE World! %d \n", 970107);

    close(fd);
    return 0;
}

static int write_file(void)
{
    int fd = open("datafile", O_WRONLY | O_CREAT, 0600); // 6(4+2) rw, 4 r, 4 r
    //The return value of open() is a file descriptor

    if(fd == -1)
    {
        printf("open() error\n");
        return -1;
    }

    // file input fuction, almost same as printf
    dprintf(fd, "Hello C World! %d \n", 970107);

    close(fd);
    return 0;
}

int main(int argc, char **argv)
{
    if(write_file())
    {
        printf("wrte_file() error !\n");
        return -1;
    }

    if(trunc_file()) // erase made by the write_file then replace by trunc_file
    {
        printf("trunc_file() error !\n");
        return -1;
    }

    if(append_file())
    {
        printf("append_file() error !\n");
        return -1;
    }

    return 0;
}