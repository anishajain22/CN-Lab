#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>

int main()
{
	int c=0;
	char buff[100];
	int p[2];
	pipe(p);
	int fd=dup(1);
	dup2(p[1],1);
	c=fork();
	if(c>0)
	{
		close(p[1]);
		read(p[0], buff, 100);
		dup2(fd, 1);
		printf("%s", buff);
	}
	else
	{
		//close(p[0]);
		char* args[]={NULL};
		execvp("./P1", args);
	}

}
