#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/shm.h>
#include<signal.h>
#include<unistd.h>

int* x;
int* y;
int childpid, ppid;
void hdfn1(int signo)
{
	if(signo==SIGUSR1)
	{
		*x=*y+1;
		fflush(stdout);
		printf("P1 is done working on X %d \n",*x);
		sleep(1);
		kill(childpid,SIGUSR2);
	}
}

void hdfn2(int signo)
{
	if(signo==SIGUSR2)
	{			
		*y=*x+1;
		fflush(stdout);
		printf("P2 is done working on Y %d \n",*y);
		sleep(1);
		kill(ppid, SIGUSR1);

	}
}

int main()
{
	int shmid;
	char *shm, *s;
	
	int shmid1, shmid2;
	
	shmid1=shmget(IPC_PRIVATE,sizeof(int),0666|IPC_CREAT);
	shmid2=shmget(IPC_PRIVATE,sizeof(int),0666|IPC_CREAT);
	
	x=(int*)shmat(shmid1,NULL,0);
	y=(int*)shmat(shmid2,NULL,0);
	*x=0;*y=0;
	
	ppid=getpid();	
	signal(SIGUSR1, hdfn1);
	signal(SIGUSR2, hdfn2);

	int c=fork();
	if(c>0)
	{
		while(1);	
	}
	else
	{
		kill(ppid, SIGUSR1);

		while(1);	
	}
	
}
