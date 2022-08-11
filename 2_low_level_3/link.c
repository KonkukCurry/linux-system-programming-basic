#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    #define TARGET_FILE "hello_world"
    // oldpath to newpath
    if(link(TARGET_FILE, "hello_by_hardlink")) // return 0 is success, 1 is fail
    {
        printf("fail to link()\n");
        return -1;
    } // hardlink create

    if(symlink(TARGET_FILE, "hello_by_symlink"))
    {
        printf("fail to symlink()\n");
        return -1;
    } // softlink create

    return 0;
}