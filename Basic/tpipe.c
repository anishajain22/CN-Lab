#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>

int main(int argc, char* argv[])
{
	char s[255]="/home/anisha/Desktop/";
	for(int i=1;i<argc;i++)
	{
		char p[255];
		strcpy(p,s);
		FILE* f1=popen(strcat(p,argv[i]),"r");
		dup2(fileno(f1), 0);
		//printf("%s", argv[i]);
	}
	
	int d;
	scanf("%d",&d);
	printf("%d",d);
	return 0;
}
	
	
