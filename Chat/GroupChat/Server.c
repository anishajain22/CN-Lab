#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<pthread.h>
#include<sys/poll.h>
char rdfo[100]="rdfo";
int pids[100]={0};
int npids;
int grps[100][100];
int count[100]={0};
struct pollfd fds[3];

void readfunc(int grp)
{
	char s[100];
	read(fds[grp].fd, s, 100);
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
	int j;
	for(j=0;j<count[grp];j++)
	{
		if(grps[grp][j]==pid)
		break;
	}
	m[k]='\0';
	if(j==count[grp])
	{
		grps[grp][count[grp]]=pid;
		count[grp]++;
		char p[100];
		sprintf(p,"%d", pid);
		char temp[100];
		strcpy(temp, rdfo);
		mkfifo(strcat(temp, p), 0666);
		for(int k=0;k<count[grp]-1;k++)
		{
			sprintf(p,"%d", grps[grp][k]);
			char temp[100];
			strcpy(temp, rdfo);
			int fd=open(strcat(temp,p),O_RDWR);
			write(fd, m, 100);
			close(fd);
		}
	}
	else
	{
		for(int k=0;k<count[grp];k++)
		{
			char p[100];
			if(grps[grp][k]!=pid)
			{printf("in this\n");
			sprintf(p,"%d", grps[grp][k]);
			char temp[100];
			strcpy(temp, rdfo);
			
			int fd=open(strcat(temp,p),O_RDWR);
			write(fd, m, 100);
			close(fd);
			}
		}
	}
	printf("done reading\n");
}

int main()
{
	npids=0;
	int fd;
	char s[100];
	mkfifo("grp0", 0666);
	mkfifo("grp1", 0666);
	mkfifo("grp2", 0666);

	int grp0=open("grp0", O_RDWR);
	int grp1=open("grp1", O_RDWR);
	int grp2=open("grp2", O_RDWR);

	
	
	fds[0].fd=grp0;
	fds[0].events=POLLIN;

	fds[1].fd=grp1;
	fds[1].events=POLLIN;

	fds[2].fd=grp2;
	fds[2].events=POLLIN;
	int j;
	while(1)
	{
		if((j=poll(fds, 3, 2000))>0)
		{
			if(fds[0].revents & POLLIN)
			{
				printf("read from group0\n");
				readfunc(0);
				
			}
			if(fds[1].revents & POLLIN)
			{
				printf("read from group1\n");
				readfunc(1);
			}
			if(fds[2].revents & POLLIN)
			{
				printf("read from group2\n");				
				readfunc(2);
			}
		}
		
	}

}


