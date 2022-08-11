#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

// easy macro
// e.g. TYPE2STR(DT_BLK) --> return "block device"
#define TYPE2STR(X) \
((X) == DT_BLK      ? "block device" :\
(X) == DT_CHR       ? "char device" :\
(X) == DT_DIR       ? "directory" :\
(X) == DT_FIFO      ? "fifo" :\
(X) == DT_LNK       ? "symlink" :\
(X) == DT_REG       ? "regular file" :\
(X) == DT_SOCK      ? "socket" :\
(X) == DT_UNKNOWN   ? "unknown" :\
"unknown")

int main(int argv, char** argc)
{
    DIR *dp = opendir("."); // . is current location
    if(!dp)
    {
        printf("opendir() error\n");
        return -1;
    }
    // error -> NULL
    // EDF -> NULL
    struct dirent *entry = readdir(dp); // readdir return is struct dirent
    while((entry = readdir(dp))) // until entry is NULL
    {
        printf("%s : %s\n", entry->d_name, TYPE2STR(entry->d_type));
    }

    closedir(dp);
    return 0;
}