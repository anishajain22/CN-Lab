#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<pthread.h>
int main()
{
int rdfd1, rdfd2;
			
			char  a[100], b[100];
			
			rdfd1=open("P1.c", O_RDONLY);
			memset(a,0,100);
			read(rdfd1, a, 100);
			close(rdfd1);
			fflush(stdout);
			printf("P %sa\n",a);
			system("gcc P1.c -o P1");
			system("./P1");
}
