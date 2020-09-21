#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<signal.h>

void hdfn()
{
	printf("Child function terminated\n");
}
int main()
{
	
	signal(SIGCHLD, hdfn);
	int c=fork();
	if(c>0)
	{	wait(NULL);
		printf("In parent process\n");
	}
	else
	{
		exit(0);
	}
}
