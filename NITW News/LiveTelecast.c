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
char p[100];
char nltstr[100]="nlt";
char ltsstr[100]="lt";

void hdfn()
{
	int fd=open(p,O_RDWR);
	char s[100];
	while(1)
	{
		printf("enter stuff\n");
		scanf("%s",s);
		write(fd, s, 100);
		if(strcmp(s,"-1")==0)
			raise(SIGKILL);
	}

}
int main()
{
	int pid=getpid();
	sprintf(p,"%d",pid);
	mkfifo("lstffo",0666);
	int fd=open("lstffo", O_RDWR);
	write(fd, p, 100);

	mkfifo(p, 0666);



	signal(SIGUSR1, hdfn);
	while(1);
}
