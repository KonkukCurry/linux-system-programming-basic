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
#include <sys/epoll.h>


int main(int argc, char** argv)
{
    int fd = inotify_init(); // suc 1 fail 0;
    int wd1 = -1; // watch descriptor
    int wd2 = -1;
    char buf[1024] = {0,};
    int ret; // return value
    struct inotify_event *event;
    int epfd = -1;

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

    // epoll instance init
    epfd = epoll_create1(0);
    if(epfd == -1)
    {
        printf("fail to epoll_create()\n");
        return -1;
    }

    // give instance what fd you going to watch
    // int epoll_ctl(int __epfd, int __op, int __fd, struct epoll_event *)
    struct epoll_event * ep_event = (struct epoll_event *)calloc(1,sizeof(struct epoll_event));

    // for fd = inotify_init()
    ep_event->events = EPOLLIN; // EPOLLIN is readable
    ep_event->data.fd = fd; // give data of fd
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, ep_event) == -1)
    {
        printf("fail to epoll_ctl()\n");
        return -1;
    }

    // for STDIN_FILENO
    memset(ep_event, 0, sizeof(struct epoll_event)); // reset
    ep_event->events = EPOLLIN; // EPOLLIN is readable
    ep_event->data.fd = STDIN_FILENO; // give data of fd
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, ep_event) == -1)
    {
        printf("fail to epoll_ctl()\n");
        return -1;
    }

    while(1)
    {
        // int epoll_wait(int __epfd, struct epoll_event *__events, int __maxevents, int __timeout)
        memset(ep_event, 0, sizeof(struct epoll_event)); // reset
        // wati return the # of ready fd
        ret = epoll_wait(epfd, ep_event, 1, 5000);

        if(ret == -1)
        {
            printf("epoll_wait() fail\n");
            break;
        }
        else if(ret == 0)
        {
            printf("epoll_wait() timeout!\n");
        }
        else if(ret > 0)
        {
            // if sth happens, ep_event->data.fd will tell you what happened
            if(ep_event->data.fd == fd)
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
            else if(ep_event->data.fd == STDIN_FILENO)
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
    close(epfd);
    close(fd);
    close(wd1);
    close(wd2);

    return 0;
}