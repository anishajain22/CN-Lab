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
		fd=open("ffo", O_RDONLY);
		printf("broadcast message is\n");		
		read(fd, s, 20);
		printf("%s\n", s);
		close(fd);
	}
}
