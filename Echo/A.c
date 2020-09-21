#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>

int main()
{
	mkfifo("ffo", 0666);
	char a[50], b[50];
	int fd;
	while(1)
	{
		fd=open("ffo", O_WRONLY);
		fgets(a, 50, stdin); 
		write(fd, a, 50);
		close(fd);
		fd=open("ffo", O_RDONLY);
		read(fd, b, 50);
		printf("echo back %s\n",b);
		close(fd);
	
	}

}
