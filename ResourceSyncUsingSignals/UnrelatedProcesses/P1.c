#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include <stdlib.h>
#include <sys/shm.h>

#define SHM_KEY1 0x1234
#define SHM_KEY2 0x1235
int *x, *y;
int pid;
void hdfn(int signo)
{
	if(signo==SIGUSR1)
	{
		*x=*y+1;		
		printf("Signal received from P2, X value %d \n", *x);
		sleep(3);
		kill(pid, SIGUSR2);
	}
}
int main()
{
	mkfifo("ffo",0666);
	signal(SIGUSR1, hdfn);
	int shmid1, shmid2;
	
	shmid1=shmget(SHM_KEY1,sizeof(int),0666|IPC_CREAT);
	shmid2=shmget(SHM_KEY2,sizeof(int),0666|IPC_CREAT);
	
	x=(int*)shmat(shmid1,NULL,0);
	y=(int*)shmat(shmid2,NULL,0);
	*x=0;*y=0;
	int  fd=open("ffo", O_WRONLY);
	char s[100];
	sprintf(s, "%d", getpid());
	write(fd, s, 100);
	close(fd);
	fd=open("ffo", O_RDONLY);
	
	read(fd, s, 100);
	close(fd);
	pid=atoi(s);
	printf(" Process 2 ka pid %d\n", pid);
	while(1)
	{
		if(kill(pid, 0)==0)
			{kill(pid, SIGUSR2);
			break;}
	}
	while(1);


}

