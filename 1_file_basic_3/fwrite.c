#ifdef __cplusplus
extern "C"
{
#endif

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct person
{
    char name[40];
    int age;
};

static int write_to_file(void)
{
    FILE *fp = NULL;
    if(!(fp = fopen("persons","w")))
    {
        return -1;
    }

    struct person kim;
    struct person lee;

    strcpy(kim.name, "KIM");
    kim.age = 30;
    strcpy(lee.name, "LEE");
    lee.age = 26;

    if(fwrite(&kim, sizeof(struct person), 1, fp) != 1)
    {
        goto err;
    }
    if(fwrite(&lee, sizeof(struct person), 1, fp) != 1)
    {
        goto err;
    }

    fclose(fp);
    return 0;

    err:
        if(fp)
            fclose(fp);
        return -1;
}

static int read_from_file(void)
{
    FILE *fp = NULL;
    struct person persons[2];

    if(!(fp = fopen("persons","r")))
        return -1;

    if(fread(persons, sizeof(struct person), 2, fp) != 2) // return is how many you read
    {
        fclose(fp);
        return -1;
    }

    fclose(fp);

    for(int i=0; i<2; i++)
    {
        printf("name : %s age : %d\n", persons[i].name,  persons[i].age);
    }

    return 0;
}

int main(int argc, char *argv)
{
    if(write_to_file())
    {
        printf("fail to write\n");
    }

    if(read_from_file())
    {
        printf("fail to read\n");
    }

    return 0;
}

#ifdef __cplusplus
}
#endif