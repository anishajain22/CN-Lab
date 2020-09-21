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
		scanf("%s", s);
		if(strlen(s)>0)
		printf("P5 got input %s\n", s);
	}
}
