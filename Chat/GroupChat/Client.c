#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<pthread.h>
#include <sys/poll.h>
char rdfo[100]="rdfo";
char* ffoname;
char p[100];

int main()
{
	sprintf(p,"%d", getpid());
	ffoname=strcat(rdfo,p);
	mkfifo(ffoname, 0666); 
	struct pollfd fds[2];
	
	fds[0].fd=STDIN_FILENO;
	fds[0].events=POLLIN;
	fds[1].fd=open(ffoname, O_RDWR);
	fds[1].events=POLLIN;
	char s[100], p[100];
	while(1)
	{
		int j=poll(fds, 2, 2000);
		if(j<=0)
			continue;
		if(fds[0].revents && POLLIN)
		{
			printf("Enter group no\n");
			char g[100];
			int d;
			scanf("%d", &d);
			sprintf(g, "%d", d);
			char grp[100]="grp";
			char* grpffo=strcat(grp,g);
			mkfifo(grpffo, 0666);
			int fd=open(grpffo, O_RDWR);
			sprintf(p, "%d", getpid());
			scanf("%s", s);
			char* k=strcat(p,s);
			write(fd, k, 100);
			close(fd);
			printf("\n");
		}
		if(fds[1].revents && POLLIN)
		{			
			read(fds[1].fd, p, 100);
			printf("Message received %s\n",p);
		}
	}

}	
