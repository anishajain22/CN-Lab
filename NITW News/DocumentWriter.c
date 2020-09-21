#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include<unistd.h>
#include<sys/msg.h>

int main(int args,char* argv[])
{
	int wfd=open("Document.txt",O_WRONLY  | O_CREAT | O_TRUNC);
	write(wfd, argv[1], 100);

}
