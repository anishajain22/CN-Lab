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
#include<sys/sem.h>
#include<sys/msg.h>

int msqid;
int *edpid;
int fd;

struct mymesg
{
	long mtype;
	int processno;
	char mtext[100];
};

void waitsem(int semid)
{
  struct sembuf sb;
  sb.sem_num=0;
  sb.sem_op=-1;
  sb.sem_flg=0;
  semop(semid, &sb, 1);
}

void signalsem(int semid)
{
  struct sembuf sb;
  sb.sem_num=0;
  sb.sem_op=1;
  sb.sem_flg=0;
  semop(semid, &sb, 1);
}

void readFromQ()
{
	struct mymesg msg;
	printf("here\n");
	while(msgrcv(msqid,&msg,sizeof(struct mymesg),0,0)==-1);
	//printf("got msg\n");
	if(msg.mtext[0]>='0' && msg.mtext[0]<='9' && msg.mtext[1]>='0' && msg.mtext[1]<='9' && msg.mtext[2]>='0' && msg.mtext[2]<='9' && msg.mtext[3]>='0' && msg.mtext[3]<='9')
	{
		kill(*edpid, SIGUSR2);		
		char pname[4];
		for(int i=0;i<4;i++)
		pname[i]=msg.mtext[i];
		pname[4]='\0';
		mkfifo(pname, 0666);
		kill(atoi(pname), SIGUSR1);
		char s[100];		
		int fd1=open(pname, O_RDWR);
		while(1)
		{
			//printf("%d\n",kill(atoi(pname),0));
			if(read(fd1,s, 100)>0)
			write(fd,s,100);
			if(strcmp(s,"-1")==0)
				break;
		}
		printf("LiveTelecastOver\n");
	}
	else
	{	
		printf("Writing on to screen\n");
		write(fd,msg.mtext,100);
	}
}
int main()
{
	key_t key;

	int sem1,sem2;
	int shmid1,shmid2;
	
	sem1 = semget(IPC_PRIVATE,1,0666|IPC_CREAT);
	sem2 = semget(IPC_PRIVATE,1,0666|IPC_CREAT);
	
	semctl(sem1,0,SETVAL,0);
	semctl(sem2,0,SETVAL,1);

	key=ftok("Edpid",65);
	int shmid=shmget(key,sizeof(int),0666|IPC_CREAT);
	edpid=shmat(shmid,NULL,0);

	key=ftok("/tmp",65);
	msqid= msgget(key, 0666|IPC_CREAT);
	//printf("msg q %d\n", msqid);
	mkfifo("Scrn",0666);
	fd=open("Scrn", O_RDWR);

	int c=fork();
	if(c==0)
	{
		while(1)
		{
			waitsem(sem2);
			sleep(3);
			readFromQ();
			signalsem(sem1);
			
		}
	}
	else
	{
		while(1)
		{
			waitsem(sem1);
			sleep(3);
			readFromQ();
			signalsem(sem2);
		}
	}
	

}
