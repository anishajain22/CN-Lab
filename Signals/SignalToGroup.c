#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<signal.h>
#include<stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<poll.h>
#include <sys/ioctl.h>
#include<sys/ipc.h>
#include <sys/shm.h>
#include <termios.h>
void hdfn(int signo)
{
	if(signo==SIGIO)
	{
		printf("Handled function\n");
		
	}
}

int main()
{
    int c=0,d=0;
    c=fork();
	signal(SIGIO, hdfn);
    if(c==0)
    {
        setpgid(getpid(),getppid());
        pid_t pid1=getppid();

        d=fork();
        if(d>0)
        {
			printf("in child2\n");
			setpgid(getpid(),pid1);
			while(1);
        }
		else
		{
			printf("in child 1\n");        	
			while(1);
		}
    }
    else if(c>0){
		printf("in paren \n");        
		setpgrp();
        sleep(5);
        int fd[2];
        pipe(fd);
        fcntl(fd[0],F_SETFL, O_ASYNC | fcntl(fd[0], F_GETFL));
	fcntl(fd[0], F_SETOWN,-1*getpgrp());
        while(1)
        {
		    write(fd[1],"hi",100);
		    sleep(2);
        }
    }
        
}
