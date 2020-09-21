#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>

void writefunc()
{
	int fd;
	char a[50];
	while(1)
	{
		fd=open("pr1", O_WRONLY);
		fgets(a, 50, stdin);
		
		write(fd, a, 50);
		close(fd);
	}
}

void readfunc()
{
	int fd;	
	char b[50];
	while(1)
	{
		fd=open("pw1", O_RDONLY);
		read(fd, b, 50);
		printf("%s\n",b);
		close(fd);
	}
}

int main()
{
	mkfifo("pr1", 0666);
	mkfifo("pw1", 0666);
	pthread_t a,b;
	pthread_create(&a, NULL, &readfunc, NULL);
	pthread_create(&b, NULL, &writefunc, NULL);
	pthread_join(a,NULL);
	pthread_join(b,NULL);

}
