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
int *sid;
int msqid1,msqid2;
struct mymesg
{
	long mtype;
	int processno;
	char mtext[512];
};
int pids[100]={0};
int npids=0;
void Recv()
{
	struct msqid_ds buf;
	int d=msgctl(msqid1,IPC_STAT,&buf);
	printf("%d\n",d);
	printf("There are %d messages\n", (int)buf.msg_qnum);
	struct mymesg msg;
	msgrcv(msqid1,&msg, sizeof(struct mymesg), 1, 0);
	int pid=msg.processno;
	int i,flag=0;
	for(i=0;i<npids;i++)
	{
		printf("%d\n", pids[i]);	
	}
	for(i=0;i<npids;i++)
	{
		if(pid!=pids[i])
		{
			msg.mtype=(long)pids[i];
			//printf("send it to %d\n", pids[i]);
			msgsnd(msqid2, &msg, sizeof(struct mymesg),IPC_NOWAIT);
			kill(pids[i],SIGUSR1);			
		}
		else
		{
			flag=1;
		}			
	}
	if(flag==0)
	{
		pids[npids]=pid;
		npids++;			
	}
}
int main()
{
	key_t key1=ftok("ServeQ",65);
	key_t key2=ftok("ClientQ",65);
	key_t key3=ftok("SrvrId",65);
	signal(SIGUSR1, Recv);
	int shmid=shmget(key3, sizeof(int), 0666 |IPC_CREAT );
	sid= (int*)shmat(shmid, NULL, 0);
	*sid=getpid();

	msqid1 = msgget(key1, 0666 | IPC_CREAT | O_RDWR );
	msqid2 = msgget(key2, 0666 | IPC_CREAT );
	//printf("%d %d \n", msqid1, msqid2);
	while(1);


}
