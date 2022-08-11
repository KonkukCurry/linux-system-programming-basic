#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int write_to_file(void)
{
	FILE *fp = fopen("data","w+");
	if(fp == NULL)
	{
		perror("fopen error\n");
		return -1;
	}
	
	fputs("hello world...\n", fp);
	fclose(fp);


	return 0;
}

int read_from_file(void)
{
	FILE *fp = fopen("data","r+");
	char *buf = (char *)calloc(1, 1024*sizeof(char));
        if(fp == NULL)
        {
                perror("fopen error\n");
                return -1;
        }

	fgets(buf, sizeof(buf), fp); // put fp string into buf
        fclose(fp);

	printf("%s\n", buf);
	free(buf);

        return 0;

}

int main(int argc, char **argv)
{
	if(write_to_file())
	{
		perror("write to file\n");
		return -1;
	}
	if(read_from_file())
	{
		perror("read to file\n");
		return -1;
	}


	return 0;
}

