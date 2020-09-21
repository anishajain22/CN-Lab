
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include<unistd.h>

int main()
{
	mkfifo("ffo", 0666);
	int fd=open("ffo", O_WRONLY);
	char s[100];
	while(1)
	{
		scanf("%s",s);
		write(fd, s, 100);	
	}
}
