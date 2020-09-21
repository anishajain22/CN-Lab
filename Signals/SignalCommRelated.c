#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<signal.h>
int pid1, pid2;
void hdfn(int signo)
{
	printf("Singal received %d\n", signo);
	if(signo==SIGUSR1)
	{
		printf("Signal from parent process\n");
		sleep(3);
		kill(pid1, SIGUSR2);
	}
	if(signo==SIGUSR2)
	{
		printf("Signal from child process\n");
		sleep(3);
		kill(pid2, SIGUSR1);
	}
}

int main()
{
	pid1=getpid();
	int c=fork();
	signal(SIGUSR2,hdfn);
	signal(SIGUSR1,hdfn);
	if(c>0)
	{
		pid2=c;
		wait(NULL);
		kill(c,SIGUSR1);
		while(1);
	}
	else
	{	kill(getppid(), SIGUSR2);
		while(1);
	}

}
