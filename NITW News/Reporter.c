#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include<unistd.h>
#include<sys/wait.h>

int *nlt;
int* lts;
int main()
{

	mkfifo("edtr", 0666);
	int fd=open("edtr", O_RDWR);
	int d=0;

	mkfifo("lstffo",0666);
	int fd2=open("lstffo", O_RDWR);

	char s[100],p[100];

	struct pollfd fds[1];
	fds[0].fd=fd2;
	fds[0].events=POLLIN;

	while(1)
	{
		printf("Enter 1 for live telecast, 2 for normal news\n");
		scanf("%d", &d);
		switch(d)
		{
			case 1:
				read(fd2,p,100);
				sleep(3);
				write(fd,p, 100);
				break;
			case 2:
				scanf("%s",s);
				write(fd, s, 100);
				break;
				
		}
	}

}
