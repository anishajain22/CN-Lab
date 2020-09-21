#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<signal.h>

int main()
{
	mkfifo("P4ffo",0666);
	char s[100];
	int  fd=open("P4ffo", O_RDONLY);
	read(fd, s, 100);
	close(fd);
	int pid=atoi(s);
	fd=open("P4ffo", O_WRONLY);
	sprintf(s, "%d", getpid());
	write(fd, s, 100);
	close(fd);
	printf("%s\n",s);
	while(1)
	{
		printf("Enter 1 to send signal\n");
		int d;
		scanf("%d", &d);
		if(d==1)
			kill(pid, SIGUSR1);

	}

}

