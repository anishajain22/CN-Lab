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
int p=2;
char hi[20]="HI";
char bye[20]="BY";
int main()
{
	struct sockaddr_in servaddr,myaddr,trainaddr;
	int sfd;
	int size=sizeof(myaddr);
	
	myaddr.sin_family = AF_INET;         
    myaddr.sin_port = htons(8003);     
    myaddr.sin_addr.s_addr = INADDR_ANY;
    
    sfd=socket(AF_INET, SOCK_DGRAM, 0);
    bind(sfd, (const struct sockaddr *)&myaddr,sizeof(myaddr));
    
    
    
    int cfd=socket(AF_INET,SOCK_STREAM,0);
    bind(cfd, (const struct sockaddr *)&myaddr,sizeof(myaddr));
    listen(cfd,5);
    
    
    
    
    struct pollfd fds[3];
    fds[0].fd=sfd;
    fds[0].events=POLLIN;
    
    fds[1].fd=cfd;
    fds[1].events=POLLIN;
    
    fds[2].fd=-1;
    fds[2].events=POLLIN;
    
    while(1)
    {
    	if(poll(fds,3,2000)>0)
    	{
    		if(fds[0].revents&POLLIN)
    		{
    			char buffer[1024];
			    int d=recvfrom(sfd,buffer,1024,MSG_WAITALL,(struct sockaddr*)&servaddr,&size);
			    buffer[d]='\0';
    			printf("train coming from %s\n",buffer);
    		}
    		if(fds[1].revents&POLLIN)
    		{
    			int fd=accept(cfd, (struct sockaddr *)&trainaddr,&size);
    			char buffer[1024];
    			int d=recv(fd, buffer,1024,0);
    			buffer[d]='\0';
    			printf("connect msg %s\n",buffer);
    			fds[2].fd=fd;
    			if(strcmp(buffer,hi)==0)
    			{
    				printf("Train compartment details\n");
    			}
    			
    		}
    		if(fds[2].fd!=-1 && (fds[2].revents&POLLIN))
    		{
    			char buffer[1024];
    			int d=recv(fds[2].fd, buffer,1024,0);
    			buffer[d]='\0';
    			printf("train is leaving\n");
    			if(strcmp(buffer,bye)==0)
    			{
					printf("Train left station\n");
					char pf[20];
					sprintf(pf,"%d",p);
					sendto(sfd,pf,20,MSG_CONFIRM,(struct sockaddr*)&servaddr,size);
    			}
    			fds[2].fd=-1;
    		}
    	
    	}
    
    }
    
    
    
    
}
