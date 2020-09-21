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
static int P5wfd;
static int P2rfd;
static int P3rfd;
char nsfd[20]="nsfd"; 
int pids[100];
int* stat;
int npids=0;
int P4pid;
void sendToAll(char s[])
{
	int out=dup(1);
	dup2(P5wfd,1);
	printf("%s\n",s);
	dup2(out,1);
}

void HandleClient(char s[])
{
	int pid=0;
	int i,k=0;
	char m[100];
	for(i=0;i<strlen(s);i++)
	{
		if(s[i]>='0' && s[i]<='9')
			pid=pid*10 + (s[i] -'0');
		else{
			while(i<strlen(s))
			{
				m[k++]=s[i++];
			}
		}		
	}
	m[k]='\0';
	int j;
	for(j=0;j<npids;j++)
	{
		if(pids[j]==pid)
			break;
	}
	//printf("%d %d\n", j, npids);
	if(j==npids)
	{
		pids[npids]=pid;
		npids++;
		char p[100];
		sprintf(p,"%d", pid);
		char temp[100];
		strcpy(temp, nsfd);
		mkfifo(strcat(temp, p), 0666);
		for(int k=0;k<npids-1;k++)
		{
			if(stat[k]==0)
			{
				sprintf(p,"%d", pids[k]);
				char temp[100];
				strcpy(temp, nsfd);
				int fd=open(strcat(temp,p),O_WRONLY);
				write(fd, m, 100);
				close(fd);
			}
		}
	}
	else
	{
		for(int k=0;k<npids;k++)
		{
			char p[100];
				if(pids[k]!=pid && stat[k]==0)
				{
				sprintf(p,"%d", pids[k]);
				char temp[100];
				strcpy(temp, nsfd);
				int fd=open(strcat(temp,p),O_WRONLY);
				write(fd, m, 100);
				close(fd);}
		}
	}

}

void hdfnchld()
{
	printf("Reconnecting to server\n");
}
void hdfn()
{
	printf("handling kill\n");
	for(int i=0;i<npids;i++)
	{
		if(stat[i]==0)
		{
			stat[i]=1;
			char f[100];			
			sprintf(f,"nsfd%d",pids[i]);
			mkfifo(f,0666);
			int fd=open(f,O_WRONLY);
			
			int p=0;		
			if(fork()==0)
			{
				while(p<10)
				{
					fd=open(f,O_WRONLY);
					char s[100];
					write(fd,"Hello",100);
					sleep(3);
					close(fd);
					p++;
				}
				stat[i]=0;
				exit(1);
			}
			break;
		}
	}
	
}
void makeConnectionP4()
{
	int  fd=open("P4ffo", O_WRONLY);
	char s[100];
	sprintf(s, "%d", getpid());
	write(fd, s, 100);
	close(fd);
	fd=open("P4ffo", O_RDONLY);
	read(fd, s, 100);
	close(fd);
	P4pid=atoi(s);

}
int main()
{
	printf("\n");
	char p[100];
	
	int shmid;
	
	shmid=shmget(IPC_PRIVATE,sizeof(int)*100,0666|IPC_CREAT);
	
	stat=shmat(shmid,NULL,0);
	
	
	P3rfd=fileno(popen("./P3", "r"));
	P5wfd=fileno(popen("./P5", "w"));
	mkfifo("P2ffo", 0666);
	mkfifo("sdfo", 0666);
	mkfifo("P4ffo",0666);
	mkfifo("recon",0666);

	sleep(3);
	signal(SIGUSR1, hdfn);
	makeConnectionP4();
	int pp[2];
	pipe(pp);
	char s[100];
	struct pollfd fds[6];
	fds[0].fd=STDIN_FILENO;
	fds[0].events=POLLIN;
	
	fds[1].fd=pp[0];
	fds[1].events=POLLIN;
	
	fds[2].fd=open("P2ffo", O_RDWR);
	fds[2].events=POLLIN;
	
	fds[3].fd=P3rfd;
	fds[3].events=POLLIN;
	
	fds[4].fd=open("sdf",O_RDWR);
	fds[4].events=POLLIN;

	fds[5].fd=open("recon",O_RDWR);
	fds[5].events=POLLIN;
	int j;
	printf("%d\n", fds[3].fd);
	int c=fork();
	if(c==0)
	{
		dup2(pp[1],1);
		while(1)
		{
		sleep(5);
		write(1, "P1Data", 20);
		fflush(stdout);			
		sleep(5);
		}
	}
	signal(SIGCHLD,hdfnchld);
	while(1)
	{	
		
		if((j=poll(fds, 6, 2000))>0)
		{
			if(fds[4].revents&&POLLIN)
			{
				read(fds[4].fd, s, 100);
				HandleClient(s);
			}
			if(fds[1].revents&&POLLIN)
			{			
				read(fds[1].fd,s,20);
				sendToAll(s);
			}				
			if(fds[0].revents&&POLLIN)
			{
				scanf("%s", s);
				sendToAll(s);
			}
			if(fds[2].revents&&POLLIN)
			{
				read(fds[2].fd, s, 100);
				sendToAll(s);
			}
			if(fds[3].revents&&POLLIN)
			{			
				read(fds[3].fd,s,100);
				sendToAll(s);
			}
			if(fds[5].revents&&POLLIN)
			{
				read(fds[5].fd,s,100);
				stat[atoi(s)]=0;
				printf("%d connected back to server\n", atoi(s));
			}
		}
			
	}

}
