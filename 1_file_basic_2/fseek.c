#include <stdio.h>
#include <string.h>
#include <stdlib.h>


static int create_file(void)
{
	FILE *fp;

	if(!(fp = fopen("datafile", "w")))
	{
		printf("fopen() file\n");
		return -1;
	}

	printf("after fopen(). offset = %ld\n", ftell(fp));
	fputs("hello world\n", fp);
	fputs("hello world!\n", fp);
	fputs("hello world!!\n", fp);
	fputs("hello world!!!\n", fp);
	printf("after fclose(). offset = %ld\n", ftell(fp));
	printf("------------------------------------------------\n");
	fclose(fp);

	return 0;
}

static int read_file(void)
{
	FILE *fp;
	if(!(fp = fopen("datafile", "r+")))
        {
                printf("fopen() file\n");
                return -1;
        }

	char *buf = (char *)calloc(1,1024*sizeof(char));

        printf("after fopen(). offset = %ld\n", ftell(fp));
        fgets(buf, sizeof(buf), fp);
        printf("after fclose(). offset = %ld\n", ftell(fp));
	printf("------------------------------------------------\n");

	fseek(fp, 0, SEEK_END); // 1 file pointer 2 how long 3 how to move
	printf("after fseek(). offset = %ld\n", ftell(fp));
	fputs("final\n", fp);
	printf("before fclose(). offset = %ld\n", ftell(fp));
	printf("------------------------------------------------\n");




        fclose(fp);
}

int main(int argc, char **argv)
{
	create_file();
	read_file();

	return 0;
}
