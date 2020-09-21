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
#include <sys/socket.h> 
#include <unistd.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include<poll.h>
int* sid;
int msqid1, msqid2;
int sfd1,sfd2;
char Quit[1024]="Quit";
struct mymesg
{
	long mtype;
	int processno;
	char mtext[512];
};


void Recv()
{
	struct pollfd fds[2];
	fds[0].fd=0;
	fds[0].events=POLLIN;
	
	fds[1].fd=sfd2;
	fds[1].events=POLLIN;
	char s[1024];
	
	while(1)
	{
		if(poll(fds,2,2000)>0)
		{
			if(fds[0].revents&POLLIN)
			{
				gets(s,1024,stdin);
				if(strcmp(s,Quit)==0)
				{
					struct msqid_ds buf;
					int d=msgctl(msqid1,IPC_STAT,&buf);
					printf("%d\n",d);
					printf("There are %d messages\n", (int)buf.msg_qnum);
					if((int)buf.msg_qnum>0)
					{
						struct mymesg msg;
						msgrcv(msqid1,&msg, sizeof(struct mymesg), 0, 0);
						int pid=(int)msg.mtype;
						printf("PID %d\n",pid);
						kill(pid,SIGUSR1);
						msg.mtype=(long)getpid();
						msgsnd(msqid1, &msg, sizeof(struct mymesg),IPC_NOWAIT);
						
						return;
					}
				}
				else
				{
					write(sfd1,s,1024);
				}
			}
			if(fds[1].revents&POLLIN)
			{
				read(fds[1].fd,s,1024);
				printf("Received: %s\n",s);
			}
		}
	}
}

void hdfn()
{
	printf("Connected to server\n");
	Recv();
}


int main()
{
	
	printf("PID %d\n", getpid());
	
	key_t key1=ftok("ServeQ",65);
	
	signal(SIGUSR1,hdfn);
	msqid1 = msgget(key1, 0666 | IPC_CREAT);
	
	mkfifo("sfdffo1",0666);
	sfd1=open("sfdffo1",O_RDWR);

	mkfifo("sfdffo2",0666);
	sfd2=open("sfdffo2",O_RDWR);
	struct mymesg msg;
	msg.mtype=(long)getpid();
	msgsnd(msqid1, &msg, sizeof(struct mymesg),IPC_NOWAIT);
	while(1);
	
}
