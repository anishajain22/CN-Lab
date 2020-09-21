#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<pthread.h>
char rdfo[100]="rdfo";
char* ffoname;
char p[100];

void* writefunc()
{
	int fd;
	char a[50];
	while(1)
	{
		fd=open("ffo", O_WRONLY);
		fgets(a, 50, stdin);
		write(fd, strcat(p, a), 50);
		close(fd);
	}
}
void* readfunc()
{
	int fd;
	char s[100];
	while(1)
	{	
		fd=open(ffoname, O_RDONLY);
		read(fd,s,100);
		printf("%s\n", s);
		close(fd); 	
	}
}

int main()
{

	mkfifo("ffo", 0666);
	sprintf(p, "%d", getpid());
	printf("%s\n", p);
	
	ffoname=strcat(rdfo,p);
	printf("%s\n",ffoname); 
	mkfifo(ffoname, 0666); 

	pthread_t read, write;
	pthread_create(&read, NULL,&readfunc, NULL);
	pthread_create(&write, NULL, &writefunc, NULL);
	pthread_join(read,NULL);
	pthread_join(write,NULL);

}
