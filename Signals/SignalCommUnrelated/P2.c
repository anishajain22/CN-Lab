
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<signal.h>

void hdfn(int signo)
{
	if(signo==SIGUSR2)
	{
		printf("Signal received from P1\n");	
	}
}
int main()
{
	mkfifo("ffo",0666);
	signal(SIGUSR2, hdfn);
	char s[100];
	int  fd=open("ffo", O_RDONLY);
	read(fd, s, 100);
	close(fd);
	int pid=atoi(s);
	printf(" Process 1 ka pid %d\n", pid);
	fd=open("ffo", O_WRONLY);
	sprintf(s, "%d", getpid());
	write(fd, s, 100);
	close(fd);
	
	while(1)
	{
		if(kill(pid, 0)==0)
			kill(pid, SIGUSR1);
		sleep(10);
	}

}

