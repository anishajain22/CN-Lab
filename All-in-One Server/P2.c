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
	mkfifo("P2ffo", 0666);
	char s[100];
	int fd=open("P2ffo",O_WRONLY);
	while(1)
	{
		fd=open("P2ffo",O_WRONLY);
		scanf("%s", s);
		write(fd, s, 100);
		close(fd);
	}
}
