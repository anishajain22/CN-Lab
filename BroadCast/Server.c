#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>

int main()
{
	mkfifo("ffo",0666);
	int fd;
	char s[20];
	while(1)
	{
		printf("enter broadcast message\n");
		fd=open("ffo", O_WRONLY);				
		fgets(s, 20, stdin);
		write(fd,s, 20);
		close(fd);
	}
}
