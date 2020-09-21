#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/ioctl.h>

int main()
{
	int fd=open("file.txt", O_WRONLY | O_APPEND);
	int nfd=dup(1);
	printf("Hii \n");
	dup2(fd,1);
	printf("Why\n");
	dup2(nfd,1);
	printf("BYe\n");
}
