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
#include <sys/inotify.h>

int main(int argc, char** argv)
{
    int fd = inotify_init(); // suc 1 fail 0;
    int wd1 = -1; // watch descriptor
    int wd2 = -1;
    // char* buf = (char*)calloc(1,1024*sizeof(char));
    char buf[1024] = {0,};
    int ret; // return value
    struct inotify_event *event;

    if(fd == -1)
    {
        printf("inotify_init fail()\n");
        return -1;
    }

    // fd, pathname, mask
    wd1 = inotify_add_watch(fd, ".", IN_CREATE|IN_DELETE); // for current location
    if(wd1 == -1)
    {
        printf("current location inotify_add_watch() fail\n");
        close(wd1);
        return -1;
    }

    wd2 = inotify_add_watch(fd, "../3_file_handle_4", IN_CREATE|IN_DELETE); // for another one
    if(wd2 == -1)
    {
        printf("/3_file_handle_4 inotify_add_watch() fail\n");
        close(wd2);
        return -1;
    }

    // after this we have to read()
    // struct inotify_event {
    //            int      wd;       /* Watch descriptor */
    //            uint32_t mask;     /* Mask describing event */
    //            uint32_t cookie;   /* Unique cookie associating related
    //                                  events (for rename(2)) */
    //            uint32_t len;      /* Size of name field */
    //            char     name[];   /* Optional null-terminated name */
    //        };
    // buf will have inotify_event struct array
    // read() return number of bytes read
    while(1)
    {
        ret = read(fd, buf, sizeof(buf));
        if(ret == -1)
        {
            printf("read() fail\n");
            break;
        }

        // after read()
        event = (struct inotify_event *)&buf[0]; // put structure
        while(ret > 0)
        {
            ret -= (sizeof(struct inotify_event) + event->len);

            if(event->mask & IN_CREATE)
            {
                printf("file %s has created\n", event->name);
            }
            if(event->mask & IN_DELETE)
            {
                printf("file %s has deleted\n", event->name);
            }
            event = (struct inotify_event*)((char*)event + sizeof(struct inotify_event) + event->len);
        }
    }

    // free(buf);
    close(fd);
    close(wd1);
    close(wd2);

    return 0;
}