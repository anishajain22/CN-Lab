#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<signal.h>
#include<string.h>
#include<sys/msg.h>

#define n 1024

struct msq{
	long type;
	char msg[n];
};

int msqid1,msqid2;

void mread(int signum,siginfo_t* info,void* ucontext){
	int pid=(int)info->si_pid;
	kill(pid,SIGUSR1);
	printf("read signal received\n");
	struct msq message;
	struct msqid_ds ds;
	msgctl(msqid2,IPC_STAT,&ds);
	if(ds.msg_qnum){
		msgrcv(msqid2,&message,sizeof(message),0,0);
		if(message.type!=(long)getpid())
			msgsnd(msqid2,&message,sizeof(message),0);
		else
			printf("message from %ld : %s\n",message.type,message.msg);
		bzero((void *)&message,sizeof(message));
	}
	kill(pid,SIGUSR2);
}

void mwrite(int signum,siginfo_t* info,void* ucontext){
	int pid=(int)info->si_pid;
	kill(pid,SIGUSR1);
	printf("write signal received\n");
	printf("Do you want to send a message ? [y/n] ");
	char c;
	scanf("%c",&c);
	if(c=='y'){
		struct msq message;
		char buf[n];
		printf("Enter message type : ");
		scanf("%ld",&message.type);
		printf("Enter message : ");
		fgets(buf,n,stdin);
		//scanf("%s",buf);
		strcpy(message.msg,buf);
		msgsnd(msqid2,&message,sizeof(message),0);
	}
	kill(pid,SIGUSR2);
}


int main(){
	key_t key1,key2;
	key1=ftok("R",7);
	key2=ftok("F",5);
	printf("key1 = %d\n",key1);
	printf("key2 = %d\n",key2);
	msqid1=msgget(key1,0666|IPC_CREAT);
	msqid2=msgget(key2,0666|IPC_CREAT);
	struct msq pidmessage;
	pidmessage.type=(long)getpid();
	printf("%ld\n",pidmessage.type);
	strcpy(pidmessage.msg,"A");
	msgsnd(msqid1,&pidmessage,sizeof(pidmessage),0);
	bzero((void *)&pidmessage,sizeof(pidmessage));
	//msgq1 for token
	//msgq2 for msg
	struct sigaction sa1,sa2;
	sa1.sa_flags=SA_SIGINFO;
	sa1.sa_sigaction=&mread;
	sigaction(SIGUSR1,&sa1,NULL);
	sa2.sa_flags=SA_SIGINFO;
	sa2.sa_sigaction=&mwrite;
	sigaction(SIGUSR2,&sa2,NULL);
	while(1);
}
