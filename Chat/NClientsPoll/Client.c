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

	mkfifo("ffo", 0666);
	sprintf(p, "%d", getpid());
	printf("%s\n", p);

	
	ffoname=strcat(rdfo,p);
	//printf("%s\n",ffoname); 
	mkfifo(ffoname, 0666); 
	printf("done here1\n");
	struct pollfd fds[2];
	
	fds[0].fd=STDIN_FILENO;
	fds[0].events=POLLIN;
	printf("done here2\n");
	fds[1].fd=open(ffoname, O_RDWR);
	fds[1].events=POLLIN;
	printf("done here3\n");
	char s[100], p[100];
	while(1)
	{
		//printf("in poll\n");
		int j=poll(fds, 2, 2000);
		if(j<=0)
			continue;
		if(fds[0].revents && POLLIN)
		{
			fgets(s, 100, stdin);
			printf("can write input\n");
			int fd=open("ffo",O_RDWR);
			sprintf(p, "%d", getpid());
			char* g;
			g=strcat(p,s);
			//printf("%s msg sent\n", g);
			write(fd, g, 100);
			close(fd);
		}
		if(fds[1].revents && POLLIN)
		{
			//printf("reading from sever\n");			
			read(fds[1].fd, p, 100);
			printf("%s\n",p);
		}
	}

}
