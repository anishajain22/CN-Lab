#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/shm.h>
int* sid;
int msqid1, msqid2;
struct mymesg
{
	long mtype;
	int processno;
	char mtext[512];
};

void hdfn()
{
	printf("HERE\n");
	struct mymesg msg;
	msgrcv(msqid2, &msg, sizeof(struct mymesg), (long)getpid(), 0);
	printf("Received %s\n", msg.mtext);
}


int main()
{

	key_t key3=ftok("SrvrId",65);
	int shmid=shmget(key3, sizeof(int), 0666 |IPC_CREAT);
	sid= (int*)shmat(shmid, NULL, 0);
	
	printf("PID %d\n", getpid());
	
	key_t key1=ftok("ServeQ",65);
	key_t key2=ftok("ClientQ",65);
	signal(SIGUSR1,hdfn);
	msqid1 = msgget(key1, 0666 | IPC_CREAT);
	msqid2 = msgget(key2, 0666 | IPC_CREAT );
	struct mymesg msg;
	char s[100];
	while(1)
	{
		printf("Enter text to send\n");
		scanf("%s",s);
		//printf("Got text\n");
		strcpy(msg.mtext, s);
		msg.mtype=1;
		msg.processno=getpid();
		msgsnd(msqid1, &msg, sizeof(struct mymesg), 0);
		kill(*sid, SIGUSR1);
	}
}
