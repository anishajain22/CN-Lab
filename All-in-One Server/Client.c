#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include<unistd.h>
char nsfd[20]="nsfd";
char pid[10];
/*void hdfn()
{
	printf("Now connecting to echo server\n");
	int p[2];
	
	char s[20], t[20];
	
	int rd=dup(0);
	while(1)
	{
		pipe(p);
		scanf("%s",s);
		if(!strcmp(s,"-1"))
		{
			printf("Connecting back to server\n");
			return;
		}
		dup2(p[0],0);
		
		write(p[1],s,20);
		close(p[1]);
		
		FILE* F=popen("./E","r");
		if(F==NULL)
			printf("Error\n");
		
		fgets(t,20,F);
		pclose(F);
		printf("%s\n",t);
		close(fileno(F));
		dup2(rd,0);
		}
}*/
int main()
{
	
	sprintf(pid,"%d", getpid());
	strcat(nsfd,pid);
	mkfifo(nsfd,0666);
	printf("nsfd fifo is %s\n", nsfd);
	mkfifo("sdfo", 0666);
	mkfifo("recon",0666);
	//signal(SIGUSR2, hdfn);
	int fd=open("sdf",O_RDWR);
	struct pollfd fds[2];
	
	fds[0].fd=STDIN_FILENO;
	fds[0].events=POLLIN;
	
	fds[1].fd=open(nsfd,O_RDWR);
	fds[1].events=POLLIN;
	int j;
	char s[100],p[100];
	while(1)
	{
		if((j=poll(fds, 2, 2000))>0)
		{
			//printf("INNNNNN\n");
			if(fds[0].revents&&POLLIN)
			{
				scanf("%s",s);
				printf("can write input\n");
				int fd=open("sdf",O_RDWR);
				char g[100];
				strcpy(g,pid);
				strcat(g,s);
				write(fd, g, 100);
				close(fd);
				fflush(stdout);
			}
			if(fds[1].revents&&POLLIN)
			{
				read(fds[1].fd, p, 100);
				printf("%s\n",p);
			}
		}
	
	}
	
	 

}
