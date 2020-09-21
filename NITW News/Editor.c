#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include<unistd.h>
#include<sys/msg.h>
int* edpid;
int* nlt;
struct mymesg
{
	long mtype;
	int processno;
	char mtext[100];
};
void hdfn()
{
	*nlt=*nlt+1;
	printf("LiveTelecastGoingOn %d\n",*nlt);
}
int msqid;
int main()
{
	signal(SIGUSR2, hdfn);
	
	int fd=open("edtr", O_RDWR);

	key_t key=ftok("/tmp",65);
	msqid = msgget(key, 0666 | IPC_CREAT);
	
	key=ftok("edpid",65);
	int shmid=shmget(key, sizeof(int), 0666| IPC_CREAT);
	edpid=shmat(shmid,NULL,0);
	*edpid=getpid();
	
	
	key=ftok("nlt",65);
	shmid=shmget(key, sizeof(int),0666|IPC_CREAT);
	nlt=shmat(shmid,NULL,0);
	
	
	
	char s[100];
	
	
	
	while(1)
	{
		read(fd, s, 100);
		if(s[0]=='/' && s[1]=='d')
		{
			char* args[]={s};
			int c=fork();
			if(c==0)execv("./D",args);
			
		}
		else
		{
			printf("adding in Queue\n");
			struct mymesg msg;
			msg.mtype=1;
			strcpy(msg.mtext, s);
			int d=msgsnd(msqid, &msg, sizeof(struct mymesg), 0);
			printf("sent %d\n",d);
		}

	}


}
