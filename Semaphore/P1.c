// #include<stdio.h>
// #include<unistd.h>
// #include<stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <sys/shm.h>
// #include <errno.h>
// #include <sys/ipc.h>

void wait(int semid)
{
  struct sembuf sb;
  sb.sem_num=0;
  sb.sem_op=-1;
  sb.sem_flg=0;
  semop(semid, &sb, 1);
}

void signal(int semid)
{
  struct sembuf sb;
  sb.sem_num=0;
  sb.sem_op=1;
  sb.sem_flg=0;
  semop(semid, &sb, 1);
}


int main()
{
	int shmid;
	char *shm, *s;
	int sem1,sem2;
	int shmid1, shmid2;
	
	int* x;
	int* y;
// create a new semaphore
	sem1 = semget(IPC_PRIVATE,1,0666|IPC_CREAT);
	sem2 = semget(IPC_PRIVATE,1,0666|IPC_CREAT);
	
	shmid1=shmget(IPC_PRIVATE,sizeof(int),0666|IPC_CREAT);
	shmid2=shmget(IPC_PRIVATE,sizeof(int),0666|IPC_CREAT);
	
	x=(int*)shmat(shmid1,NULL,0);
	y=(int*)shmat(shmid2,NULL,0);
		
	semctl(sem1,0,SETVAL,0);
	semctl(sem2,0,SETVAL,0);
	int c=fork();
	if(c>0)
	{
		int p=0;
		while(1)
		{
			
			*x=p;
			sleep(3);
			signal(sem1);
			wait(sem2);
			p=*y;
			p=p+5;
			printf("process 1 is printing %d\n",p);
		}
	}
	else
	{
		int l=0;
		while(1)
		{
			wait(sem1);
			l=*x;
			l=l+10;
			printf("process 2 is printing %d\n",l);
			*y=l;
			sleep(3);
			signal(sem2);
		}
	}
	
}
