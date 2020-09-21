#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<pthread.h>

int main()
{
	int p[2];
	int in=dup(0);
	int out=dup(1);
	
	char fname[100];
	scanf("%s",fname);
	
	
	int coutfd=open("O1.txt",O_CREAT | O_WRONLY |O_TRUNC|O_RDONLY);
	dup2(coutfd,1);
	
	
	int fdi=open("T.txt",O_RDONLY);
	
	system("chmod 777 O1.txt");
	system("gcc P1.c -o P1");
	
	int c=0;
	c=fork();
	
	if(c>0)
	{
		wait(NULL);
		close(coutfd);
		int soutfd=open("O2.txt",O_CREAT | O_WRONLY |O_TRUNC);
		dup2(soutfd,1);
		system("chmod 777 O2.txt");
		system("gcc Sol.c -o Sol");
		c=fork();
		if(c>0)
		{
			wait(NULL);
			close(soutfd);
			
			dup2(out,1);
			int rdfd1, rdfd2;
			
			char  a[100], b[100];
			memset(a, 0,100);
			rdfd1=open("O1.txt", O_RDONLY);
			read(rdfd1, a, 100);
			close(rdfd1);
			fflush(stdout);


			rdfd2=open("O2.txt", O_RDONLY);
			read(rdfd2, b, 100);
			close(rdfd2);
			
			if(!strcmp(a,b))
			{
				printf("Success\n");
			}
			else
			{
				printf("Failed test cases\n");
			}
		}
		else
		{
			fseek(fdopen(fdi,"r"), 0, SEEK_SET);
			dup2(fdi,0);
			char* args[]={NULL};
			execvp("./Sol",args);
		}
		
		
	}
	else
	{	
		fseek(fdopen(fdi,"r"), 0, SEEK_SET);
		dup2(fdi,0);
		char* args[]={NULL};
		execvp("./P1", args);
	}
}	
