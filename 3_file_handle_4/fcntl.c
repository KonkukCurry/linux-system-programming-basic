#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> // for open function
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/file.h> // for flock

int main(int argc, char** argv)
{
    // STDIN_FILENO standard input
    // STDOUT_FILENO standard output
    // STDERR_FILENO standard error
    
    char *buf = (char*)calloc(1,128*sizeof(char));
    int ret = read(STDIN_FILENO, buf, sizeof(buf));
    // return # of bytes read
    // default is blocking operation
    printf("read() [%s] %d bytes\n", buf, ret);

    free(buf);
    return 0;
}