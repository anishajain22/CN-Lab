#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>

int main()
{
	int c=0;
	int pp1[2],pp2[2];
	char output[50], buff[20],cont[50];
	int n=50;
	pipe(pp1);
	pipe(pp2);
	char file[20];
	printf("Enter file name \n");
	scanf("%s", file);
	c=fork();
	if(c>0)
	{
		close(pp1[0]);
		close(pp2[1]);
		printf("Writing file name to pipe 1 \n");
		write(pp1[1], file, strlen(file)+1);
		close(pp1[1]);
		wait(NULL);
		printf("Reading contents form pipe 2 \n");
		read(pp2[0], output, n);
		printf("Output is %s \n", output);
	}
	else{
		close(pp1[1]);
		close(pp2[0]);
		printf("Reading file name from pipe 1 \n");
		read(pp1[0], buff, n);
		int fd=open(buff, O_RDONLY);
		close(pp1[0]);
		read(fd, cont, n);
		printf("Writing file content to pipe 2\n");
		write(pp2[1], cont, n);
	}
	return 0; 
}
