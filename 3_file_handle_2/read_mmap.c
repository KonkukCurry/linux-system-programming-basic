#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> // for open function
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

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
    int fd;
    fd = open("person_info", O_RDONLY);
    if(fd == -1)
    {
        printf("open() fail\n");
        close(fd);
        return -1;
    }

    struct stat statbuf;
    if(fstat(fd, &statbuf) == -1)
    {
        printf("fail to fstat()\n");
        close(fd);
        return -1;
    }

    // void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    // if adrr is NULL, we are just letting kernel to pick the address
    // length is file size, use fstat -> st_size
    // offset 0 means it starts from the file start part
    // return success : MAPPED ADDRESS
    // return fail : "MAP_FAILED"
    struct person *p = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED)
    {
        printf("Mapping Failed()\n");
        close(fd);
        return -1;
    }
    
    for(int i = 0; i < statbuf.st_size / sizeof(struct person) ; i++)
    {
        printf("name : %s age : %d\n", (p+i)->name, (p+i)->age);
    }

    munmap(p, statbuf.st_size); // like free
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