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
	int fd=open("ffo", O_RDONLY);
	
	struct pollfd fds[2];
	
	fds[0].fd=fd;
	fds[0].events=POLLIN;

	fds[1].fd=STDIN_FILENO;
	fds[1].events=POLLIN;
	
	char s[100];
	while(1)
	{
		int j=poll(fds, 2, 2000);
		if(j<=0)
			continue;
		if(fds[0].revents && POLLIN)
		{
			printf("can take input from fifo\n");
			read(fds[0].fd, s, 100);
			printf("0 %s\n",s);
		}
		if(fds[1].revents && POLLIN)
		{
			printf("can take input form keyboard\n");
			read(fds[1].fd, s, 100);
			printf("1 %s",s);
		}
	}
	
}
