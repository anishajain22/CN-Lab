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
int flag=0;

void hdfn1(){
	while(flag!=1);
}

void hdfn2(){
	flag=1;
}


int main(){
	key_t key1,key2;
	key1=ftok("R",7);
	key2=ftok("F",5);
	printf("key1 = %d\n",key1);
	printf("key2 = %d\n",key2);
	int msqid1,msqid2;
	msqid1=msgget(key1,0666|IPC_CREAT);
	msqid2=msgget(key2,0666|IPC_CREAT);
	struct msq message1,message2;
	int flag=0;
	struct msqid_ds ds;
	msgctl(msqid2,IPC_STAT,&ds);
	signal(SIGUSR1,hdfn1);
	signal(SIGUSR2,hdfn2);
	while(1){
		if(msgrcv(msqid1,&message1,sizeof(message1),0,0)){
			//printf("1\n");
			//printf("%ld\n",ds.msg_qnum);
			if(ds.msg_qnum){
				//printf("3\n");
				/*if(message2.type==message1.type){
					msgsnd(msqid2,&message2,sizeof(message2),0);
					kill(message1.type,SIGUSR1);
				}
				else{
					msgsnd(msqid2,&message2,sizeof(message2),0);
				}*/
				kill(message1.type,SIGUSR1);
			}
			else{
				//printf("2\n");
				kill(message1.type,SIGUSR2);
			}
			msgsnd(msqid1,&message1,sizeof(message1),0);
			flag=0;
		}
	}
}
