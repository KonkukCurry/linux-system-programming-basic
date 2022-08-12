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

int main(int argc, char** argv)
{
    // STDIN_FILENO standard input
    // STDOUT_FILENO standard output
    // STDERR_FILENO standard error
    int flag = 0;
    int fd = STDIN_FILENO;
    char *buf = (char*)calloc(1,128*sizeof(char));
    int ret = read(fd, buf, sizeof(buf));
    // return # of bytes read
    // default is blocking operation
    printf("read() [%s] %d bytes\n", buf, ret);

    // if you want to change mode with keep having origin one
    // you have to get origin flag with F_GETFL
    // then you set with F_SETFL
    flag = fcntl(fd, F_GETFL);
    if(flag == -1) // check if it's error
    {
        printf("fcntl(F_GETFL) fail!\n");
        free(buf);
        return -1;
    }
    ret = fcntl(fd, F_SETFL, flag|O_NONBLOCK);
    // make non-block with keeping the original one
    if(ret == -1)
    {
        printf("fcntl(F_SETFL) fail!\n");
        free(buf);
        return -1;
    }

    printf("trying to read.....\n");
    memset(buf , 0, sizeof(buf)); // making array with all 0
    ret = read(fd, buf, sizeof(buf));
    printf("read() [%s] %d bytes\n", buf, ret);
    if(ret == -1)
    {
        printf("error # = %d, %s\n",errno, strerror(errno));
    }

    free(buf);
    return 0;
}