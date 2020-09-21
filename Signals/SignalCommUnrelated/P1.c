#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<signal.h>
void hdfn(int signo)
{
	if(signo==SIGUSR1)
	{
		printf("Signal received from P2\n");
	}
}
int main()
{
	mkfifo("ffo",0666);
	signal(SIGUSR1, hdfn);
	int  fd=open("ffo", O_WRONLY);
	char s[100];
	sprintf(s, "%d", getpid());
	write(fd, s, 100);
	close(fd);
	fd=open("ffo", O_RDONLY);
	
	read(fd, s, 100);
	close(fd);
	int pid=atoi(s);
	printf(" Process 2 ka pid %d\n", pid);
	while(1)
	{
		if(kill(pid, 0)==0)
			kill(pid, SIGUSR2);
		sleep(10);
	}


}

