#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>

int main()
{
	int p[2];
	
	char s[20], t[20];
	
	int rd=dup(0);
	while(1)
	{
	pipe(p);
	scanf("%s",s);
	dup2(p[0],0);
	
	write(p[1],s,20);
	close(p[1]);
	
	FILE* F=popen("/home/anisha/Desktop/Echo/EchoBack","r");
	dup2(fileno(F),0);
	
	scanf("%s",t);
	printf("%s\n",t);
	close(fileno(F));
	dup2(rd,0);
	}
	return 0;
	
}
