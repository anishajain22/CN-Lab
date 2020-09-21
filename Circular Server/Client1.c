#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include <stdio.h>  
#include <string.h>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h>
#include<sys/ioctl.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>
#include<pthread.h>
#include <sys/msg.h>
#include <sys/shm.h>
int* sid;
int msqid1, msqid2;
int sfd1,sfd2,sfd;
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
	
	fds[1].fd=sfd;
	fds[1].events=POLLIN;
	char s[1024];
	
	while(1)
	{
		if(poll(fds,2,2000)>0)
		{
			if(fds[0].revents&POLLIN)
			{
				gets(s,1024,stdin);
				printf("Input entered is: %s\n",s);
				if(strcmp(s,Quit)==0)
				{
					printf("Quiting\n");
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
						break;
					}
				}
				else
				{
					write(sfd,s,1024);
				}
			}
			if(fds[1].revents&POLLIN)
			{
				read(fds[1].fd,s,1024);
				printf("Received: %s\n",s);
			}
		}
	
	}
	
	fds[0].fd=sfd1;
	fds[0].events=POLLIN;
	
	fds[1].fd=sfd;
	fds[1].events=POLLIN;	
	while(1)
	{
		if(poll(fds,2,2000)>0)
		{
			if(fds[0].revents&POLLIN)
			{
				read(fds[0].fd,s,1024);
				send(sfd,s,1024,0);
			}
			if(fds[1].revents&POLLIN)
			{
				read(fds[1].fd,s,1024);
				write(sfd2,s,1024);
			}
		}
	}
}


void hdfn()
{
	Recv();
}

int main()
{
	
	printf("PID %d\n", getpid());
	
	key_t key1=ftok("ServeQ",65);
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;         
   	servaddr.sin_port = htons(7000);     
    servaddr.sin_addr.s_addr = INADDR_ANY;
    
    
	signal(SIGUSR1,hdfn);
	msqid1 = msgget(key1, 0644 | IPC_CREAT);
	printf("msqid %d\n",msqid1);
	mkfifo("sfdffo1",0666);
	sfd1=open("sfdffo1",O_RDWR);

	mkfifo("sfdffo2",0666);
	sfd2=open("sfdffo2",O_RDWR);

	printf("%d %d\n",sfd1,sfd2);
	sfd=socket(AF_INET, SOCK_STREAM, 0);
	if (connect(sfd, (struct sockaddr *)&servaddr,sizeof(struct sockaddr)) == -1) 
	{
		perror("Connect\n");
		return 0;
	}
	Recv();
	while(1);
	
}
