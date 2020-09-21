#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include<unistd.h>
#include<sys/msg.h>

int main()
{
	mkfifo("Scrn",0666);
	int fd=open("Scrn", O_RDWR);
	struct pollfd fds[1];
	fds[0].fd=fd;
	fds[0].events=POLLIN;
	char s[100];
	while(1)
	{
		if(poll(fds,1,2000)>0)
		{
			if(fds[0].revents&&POLLIN)
			{
				read(fds[0].fd,s,100);
				printf("%s\n",s);
			}
		}
	}
}
