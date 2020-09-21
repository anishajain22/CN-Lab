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

int pid;
int *x, *y;
void hdfn(int signo)
{
	if(signo==SIGUSR2)
	{
		*y=*x+1;		
		printf("Signal received from P1, Y value %d \n",*y);
		sleep(3);
		kill(pid, SIGUSR1);
		
	}
}
int main()
{
	mkfifo("ffo",0666);
	signal(SIGUSR2, hdfn);
	int shmid1, shmid2;
	
	shmid1=shmget(SHM_KEY1,sizeof(int),0666|IPC_CREAT);
	shmid2=shmget(SHM_KEY2,sizeof(int),0666|IPC_CREAT);
	
	x=(int*)shmat(shmid1,NULL,0);
	y=(int*)shmat(shmid2,NULL,0);

	char s[100];
	int  fd=open("ffo", O_RDONLY);
	read(fd, s, 100);
	close(fd);
	pid=atoi(s);
	printf(" Process 1 ka pid %d\n", pid);
	fd=open("ffo", O_WRONLY);
	sprintf(s, "%d", getpid());
	write(fd, s, 100);
	close(fd);
	while(1);

}

