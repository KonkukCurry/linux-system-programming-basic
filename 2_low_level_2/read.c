#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> // for open function
#include <sys/stat.h>
#include <fcntl.h>

struct person
{
    char name[40];
    int age;
};

static int write_info(struct person *p)
{
    ssize_t ret;
    int fd = open("person_info", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if(fd == -1)
    {
        printf("open() fail\n");
        close(fd);
        return -1;
    }

    ret = write(fd, p, sizeof(struct person));
    if(ret == -1)
    {
        printf("write error\n");
        close(fd);
        return -1;
    }
    else if (ret != sizeof(struct person))
    {
        printf("partial write!!\n");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

static int dump_info(void)
{
    ssize_t ret;
    struct person p;
    int fd = open("person_info", O_RDONLY);
    if(fd == -1)
    {
        printf("open() fail\n");
        close(fd);
        return -1;
    }

    do{
        ret = read(fd, &p, sizeof(struct person));
        if(ret == -1)
        {
            printf("read error\n");
            close(fd);
            return -1;
        }
        else if(ret == 0) // end of the file (return 0)
        {
            FILE *fp = fdopen(fd, "r"); // fd to FILE*
            if(fp == NULL)
            {
                printf("fdopen() fail\n");
                close(fd);
                return -1;
            }
            printf("file position = %ld\n",ftell(fp));
            // file pointer input -> current offset
            break;
        }
        else if (ret != sizeof(struct person))
        {
            printf("partial read!!\n");
            close(fd);
            return -1;
        }
        printf("name %s age %d\n", p.name, p.age);
    }while(1);

    close(fd);
    return 0;
}

int main(int argc, char **argv)
{
    struct person persons[2] = {
        {"kim", 40},
        {"finn", 26}
    };

    for(int i =0; i<2 ;i++)
    {
        write_info(&persons[i]);
    }
    
    dump_info();

    return 0;
}