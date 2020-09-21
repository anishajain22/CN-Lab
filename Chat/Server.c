#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<pthread.h>

static char ***names;

void* writefunc()
{
	int fd;
	char a[50];
	while(1)
	{
		fd=open("pw1", O_WRONLY);
		 
		if(fgets(a, 50, stdin)>0)
		{
			write(fd, a, 50);
			close(fd);
			fd=open("pw2", O_WRONLY);
			//fgets(a, 50, stdin); 
			write(fd, a, 50);
			close(fd);
			fd=open("pw3", O_WRONLY);
			//fgets(a, 50, stdin); 
			write(fd, a, 50);
		}
		close(fd);
		
	}
}

void* readfunc3()
{
	int fd;	
	char b[50],a[50];
	int t=0;	
	while(1)
	{
		fd=open("pr3", O_RDONLY);
		if((t=read(fd, b, 50))>0)
		{
			printf("Client sent %s\n",b);
			fd=open("pw2", O_WRONLY);
			write(fd, b, 50);
			close(fd);
			fd=open("pw1", O_WRONLY);
			write(fd, b, 50);
			close(fd);
		}
		close(fd);
		
	}
}
void* readfunc2()
{
	int fd;	
	char b[50],a[50];
	int t=0;	
	while(1)
	{
		fd=open("pr2", O_RDONLY);
		if((t=read(fd, b, 50))>0)
		{
			printf("Client sent %s\n",b);
			close(fd);
			fd=open("pw1", O_WRONLY);
			write(fd, b, 50);
			close(fd);
			fd=open("pw3", O_WRONLY);
			write(fd, b, 50);
			close(fd);
		}
		close(fd);
		
	}
}
void* readfunc1()
{
	int fd;	
	char b[50],a[50];
	int t=0;	
	while(1)
	{
		fd=open("pr1", O_RDONLY);
		if((t=read(fd, b, 50))>0)
		{
			printf("Client sent %s\n",b);
			close(fd);
			fd=open("pw2", O_WRONLY);
			write(fd, b, 50);
			close(fd);
			fd=open("pw3", O_WRONLY);
			write(fd, b, 50);
			close(fd);
		}
		close(fd);
		
	}
}

/*void* readfunc()
{
	char a[50], b[50];
	while(1)
	{
		int t;
		for(int i=0;i<3;i++)
		{
			int fd1=open(names[i][0],O_RDONLY);
			if((t=read(fd1, a, 50))>0)
			{	
				printf("\n");
				close(fd1);
				for(int j=0;j<3;j++)
				{
					if(i!=j)
					{
						printf("\n");
						int fd2=open(names[j][1],O_WRONLY);
						write(fd2, a, 50);
						close(fd2);
					}
				}				
			}
			close(fd1);
					
		}	
	}
}*/
int main()
{

	  
    // allocating memory for 1st dimension 
    	names = (char***)calloc(3, sizeof(char**)); 
	for (int i = 0; i < 3; i++) 
	{ 
		names[i] = (char**)calloc(2, sizeof(char*));
		names[i][0]=(char*)calloc(20, sizeof(char));
		names[i][1]=(char*)calloc(20, sizeof(char));
	} 
	names[0][0]="pr1";names[0][1]="pw1";
	names[1][0]="pr2";names[1][1]="pw2";
	names[2][0]="pr3";names[2][1]="pw3";
	

	mkfifo("pr1", 0666);
	mkfifo("pw1", 0666);
	mkfifo("pr2", 0666);
	mkfifo("pw2", 0666);
	mkfifo("pr3", 0666);
	mkfifo("pw3", 0666);
	
	pthread_t a1,a2,a3,b,a;
	pthread_create(&a1, NULL, &readfunc1, NULL);
	pthread_create(&a1, NULL, &readfunc2, NULL);
	pthread_create(&a1, NULL, &readfunc3, NULL);
	pthread_create(&b, NULL, &writefunc, NULL);
	//pthread_create(&a, NULL, &readfunc, NULL);
	pthread_join(a1,NULL);
	pthread_join(a2,NULL);
	pthread_join(a3,NULL);
	pthread_join(b,NULL);
	//pthread_join(a,NULL);
}
