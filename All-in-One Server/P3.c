#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include<unistd.h>

int main()
{
	char s[100];
	while(1)
	{
		write(1, "HellofromP3",100);
		fflush(stdout);
		sleep(5+rand()%5);		
	}
	
}
