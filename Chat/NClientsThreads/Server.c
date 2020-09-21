#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<pthread.h>
char rdfo[100]="rdfo";
int pids[100]={0};
int npids;

int main()
{
	mkfifo("ffo", 0666);
	npids=0;
	int fd;
	char s[100];
	while(1)
	{
		fd=open("ffo", O_RDONLY);		
		read(fd, s, 100);
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
		printf("%d %d\n", j, npids);
		if(j==npids)
		{
			pids[npids]=pid;
			npids++;
			char p[100];
			sprintf(p,"%d", pid);
			char temp[100];
			strcpy(temp, rdfo);
			mkfifo(strcat(temp, p), 0666);
			for(int k=0;k<npids-1;k++)
			{
				sprintf(p,"%d", pids[k]);
				char temp[100];
				strcpy(temp, rdfo);
				int fd=open(strcat(temp,p),O_WRONLY);
				write(fd, m, 100);
				close(fd);
			}
		}
		else
		{
			for(int k=0;k<npids;k++)
			{
				char p[100];
				if(pids[k]!=pid)
				{printf("in this\n");
				sprintf(p,"%d", pids[k]);
				char temp[100];
				strcpy(temp, rdfo);
				
				int fd=open(strcat(temp,p),O_WRONLY);
				write(fd, m, 100);
				close(fd);}
			}
		}
		close(fd);
		
	}

}
