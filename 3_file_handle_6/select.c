#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> // for open function
#include <sys/time.h>
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
    char buf[1024] = {0,};
    int ret; // return value
    struct inotify_event *event;
    fd_set rfds;
    struct timeval timeout;

    if(fd == -1)
    {
        printf("inotify_init fail()\n");
        return -1;
    }
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

    while(1)
    {        
        FD_ZERO(&rfds); // init
        FD_SET(fd, &rfds);
        FD_SET(STDIN_FILENO, &rfds);

        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        //int select(int __nfds, fd_set *__restrict__ __readfds, fd_set *__restrict__ __writefds,
        //                      fd_set *__restrict__ __exceptfds, struct timeval *__restrict__ __timeout)
        ret = select(fd > STDIN_FILENO ? fd + 1 : STDIN_FILENO + 1, &rfds, NULL, NULL, &timeout);
        if(ret == -1)
        {
            printf("select() fail\n");
            break;
        }
        else if(ret == 0)
        {
            printf("select() timeout!\n");
        }
        else if(ret > 0)
        {
            if(FD_ISSET(fd, &rfds))
            {
                ret = read(fd, buf, sizeof(buf));
                if(ret == -1)
                {
                    printf("read() fail\n");
                    break;
                }
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
            else if(FD_ISSET(STDIN_FILENO, &rfds))
            {
                memset(buf, 0, sizeof(buf));
                ret = read(STDIN_FILENO, buf, sizeof(buf));
                if(ret == -1)
                {
                    printf("read() fail\n");
                    break;
                }
                printf("user input [%s]\n", buf);
            }
        }        
    }

    // free(buf);
    close(fd);
    close(wd1);
    close(wd2);

    return 0;
}