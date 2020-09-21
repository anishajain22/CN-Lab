#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<pthread.h>

int main()
{

	int p[2];
	char b[100];
	pipe(p);
	dup2(p[0],0);
	
	write(p[1], "P1.c", 100);
	close(p[1]);

	printf("XXX\n");

	FILE* fp=popen("/home/anisha/Desktop/OnlineJudge/Server", "r");
	dup2(fileno(fp),0);
	
	scanf("%s",b);
	
	printf("%s\n", b);
}
