#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include <stdio.h>  
#include <string.h>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h>
#include<sys/ioctl.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>
#include<pthread.h>
int count[6]={0};
int nsfds[6][100]={-1};
int sfds[6][100]={-1};
#define numserv 5
struct funcstr
{
	int sid;
	int sfd;
};
void* servfunc(void* varg)
{
	struct funcstr f=*(struct funcstr*) varg;
	int sid=f.sid;
	struct pollfd fds[201];
	int sn_size=sizeof(struct sockaddr);
	struct sockaddr_in cliaddr;
	char buffer[20];
	int d;
	while(1)
	{	
		if(count[sid]>0)
		{
			printf("bdha\n");
		}
		if(poll(fds,201,2000)>0)
		{
			for(int i=0;i<count[sid];i++)
			{
				printf("wtf\n");
				if(fds[i].fd!=-1 && fds[i].revents&POLLIN)
				{
					d=recv(fds[i].fd,buffer,20,0);
					buffer[d]='\0';
					printf("Received from client %s\n",buffer);
					send(fds[i+100].fd,buffer,20,0);
					
				}
			}
			for(int i=100;i<count[sid]+100;i++)
			{
				if(fds[i].fd!=-1 && fds[i].revents&POLLIN)
				{
					int d=recv(fds[i].fd,buffer,20,0);
					buffer[d]='\0';
					printf("Received from server %s\n",buffer);
					send(fds[i-100].fd,buffer,20,0);
				}
			}
		}
		for(int i=0;i<count[sid];i++)
		{
			fds[i].fd=nsfds[sid][i];
			fds[i].events=POLLIN;
		}
		for(int i=0;i<count[sid];i++)
		{
			fds[i+100].fd=sfds[sid][i];
			fds[i+100].events=POLLIN;
		}
	}
	
}

int main()
{
	int cfd,psfd[numserv+1]={0};
	struct sockaddr_in myaddr,psrvaddr[numserv+1],taddr;
	
	int sn_size=sizeof(myaddr);
    
    if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    } 
    
    myaddr.sin_family = AF_INET;         
    myaddr.sin_port = htons(5000);     
    myaddr.sin_addr.s_addr = INADDR_ANY;
    
    for(int i=1;i<=numserv;i++)
    {
		psrvaddr[i].sin_family = AF_INET;  
		int psport=i+6000;       
		psrvaddr[i].sin_port = htons(psport);     
		psrvaddr[i].sin_addr.s_addr = INADDR_ANY;
		bzero(&(psrvaddr[i].sin_zero),8); 
    }
    
    bind(cfd, (struct sockaddr *)&myaddr, sizeof(myaddr));
	
	if (listen(cfd, 5) == -1) {
        perror("Listen");
        exit(1);
	}
	
	
	pthread_t pt[numserv+1];
	
	for(int i=1;i<=numserv;i++)
	{
		/*if (connect(psfd[i], (struct sockaddr *)&psrvaddr[i],sizeof(struct sockaddr)) == -1) 
		{
			perror("Connect");
			exit(1);
		}*/
		struct funcstr f;
		f.sid=i;
		/*f.sfd=psfd[i];*/
		pthread_create(&pt[i],NULL,&servfunc,(void*)&f);
		//pthread_join(pt[i],NULL);
	}
	/*for(int i=0;i<numserv;i++)
		pthread_join(pt[i],NULL);*/
		
	struct pollfd fds[10];
	fds[0].fd=cfd;
	fds[0].events=POLLIN;
	char buffer[20];
	
	printf("After creation of thread\n");
	while(1)
	{
		if(poll(fds, 1, 2000)>0)
		{
			//printf("got something\n");
			if(fds[0].revents&POLLIN)
			{
				printf("accept\n");
				fflush(stdout);
				int fd=accept(fds[0].fd, (struct sockaddr *)&taddr,&sn_size);
				printf("%d\n",fd);
				int d=recv(fd,buffer,20,0);
				buffer[d]='\0';
				int sid=atoi(buffer);
				printf("%d\n",sid);
				nsfds[sid][count[sid]]=fd;
				sfds[sid][count[sid]] = socket(AF_INET, SOCK_STREAM, 0);
				printf("%d\n",sfds[sid][count[sid]]);
				if (connect(sfds[sid][count[sid]], (struct sockaddr *)&psrvaddr[sid],sizeof(struct sockaddr)) == -1) 
				{
					perror("Connect");
					exit(1);
				}
				count[sid]++;
			}
		}
	} 
}

