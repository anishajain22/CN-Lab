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

int freePf[3]={0,0,0};
int ports[3]={8001,8002,8003};
char direcs[3][20]={"V\0","H\0","D\0"};

int main()
{
	struct sockaddr_in addr[3],servaddr,incmaddr,P1addr,P2addr,P3addr;
	int size=sizeof(incmaddr);
	int tsfd[3],sfd;
	
	tsfd[0] = socket(AF_INET, SOCK_DGRAM, 0);
	tsfd[1] = socket(AF_INET, SOCK_DGRAM, 0);
	tsfd[2] = socket(AF_INET, SOCK_DGRAM, 0);
	sfd = socket(AF_INET, SOCK_DGRAM,0);
	
	servaddr.sin_family = AF_INET;         
    servaddr.sin_port = htons(5000);     
    servaddr.sin_addr.s_addr = INADDR_ANY;
    
    addr[0]=addr[1]=addr[2]=P1addr=P2addr=P3addr=servaddr;
    
    addr[0].sin_port=htons(7001);
    addr[1].sin_port=htons(7002);
    addr[2].sin_port=htons(7003);
    
    P1addr.sin_port=htons(8001);
    P2addr.sin_port=htons(8002);
    P3addr.sin_port=htons(8003);
    
    bind(sfd, (const struct sockaddr *)&servaddr,sizeof(servaddr));
    bind(tsfd[0], (const struct sockaddr *)&addr[0],sizeof(addr[0]));
    bind(tsfd[1], (const struct sockaddr *)&addr[1],sizeof(addr[1]));
    bind(tsfd[2], (const struct sockaddr *)&addr[2],sizeof(addr[2]));
    //do with select with signal mode
    /*
	struct pollfd fds[4];
    
    fds[0].fd=tsfd[0];
    fds[0].events=POLLIN;
    fds[1].fd=tsfd[1];
    fds[1].events=POLLIN;
    fds[2].fd=tsfd[2];
    fds[2].events=POLLIN;
    fds[3].fd=sfd;
    fds[3].events=POLLIN;
    
    char buffer[20],pf[20],train[50];
    
    
    while(1)
    {
    	if(poll(fds,4,2000)>0)
    	{
    		if(fds[4].revents&POLLIN)
    		{
    			recvfrom(fds[4].fd,buffer,20,MSG_WAITALL,(struct sockaddr*)&incmaddr,&size);
    			int p=atoi(buffer);
    			freePf[p]=0;
    		}
    		for(int i=0;i<3;i++)
    		{
    			if(fds[i].revents&POLLIN)
    			{
    				int d=recvfrom(fds[i].fd,buffer,20,MSG_WAITALL,(struct sockaddr*)&incmaddr,&size);
    				buffer[d]='\0';
    				if(freePf[0]==0 || freePf[1]==0 || freePf[2]==0)
    				{
    					sendto(sfd,direcs[i],strlen(direcs[i]),MSG_CONFIRM,(struct sockaddr*)&P1addr,size);
    					sendto(sfd,direcs[i],strlen(direcs[i]),MSG_CONFIRM,(struct sockaddr*)&P2addr,size);
    					sendto(sfd,direcs[i],strlen(direcs[i]),MSG_CONFIRM,(struct sockaddr*)&P3addr,size);
    					int k;
    					for(k=0;k<3;k++)
    					{
    						if(freePf[k]==0)
    						break;
    					}
    					sprintf(pf,"%d",ports[k]);
    					freePf[k]=1;
    					sendto(tsfd[i],pf,strlen(pf)+1,MSG_CONFIRM,(struct sockaddr*)&incmaddr,size);   					    					
    				}
    				else
    				{
    					char noFree[1024]="noSpace";
    					sendto(fds[i].fd,noFree,strlen(noFree),MSG_CONFIRM,(struct sockaddr*)&incmaddr,size);
    				}
    			}
    		}
    	
    	}
    
    }
    */
    
    
    fd_set rset;
    char buffer[20],pf[20],train[50];
    int maxfdp=0;
    FD_ZERO(&rset); 
    
    while(1)
    {
    	for(int i=0;i<3;i++)
    	{
    		FD_SET(tsfd[i], &rset);
    		if(maxfdp<tsfd[i])
    		maxfdp=tsfd[i]; 
        }
        FD_SET(sfd,&rset);
        if(maxfdp<sfd)
    		maxfdp=sfd; 
        
        if(select(maxfdp, &rset, NULL, NULL, NULL)>0)
        {
        	if(FD_ISSET(sfd, &rset))
        	{
        		recvfrom(sfd,buffer,20,MSG_WAITALL,(struct sockaddr*)&incmaddr,&size);
    			int p=atoi(buffer);
    			freePf[p]=0;
        	}
        	for(int i=0;i<3;i++)
        	{
        		if (FD_ISSET(tsfd[i], &rset))
        		{
        			int d=recvfrom(tsfd[i],buffer,20,MSG_WAITALL,(struct sockaddr*)&incmaddr,&size);
    				buffer[d]='\0';
    				if(freePf[0]==0 || freePf[1]==0 || freePf[2]==0){
    					sendto(sfd,direcs[i],strlen(direcs[i]),MSG_CONFIRM,(struct sockaddr*)&P1addr,size);
    					sendto(sfd,direcs[i],strlen(direcs[i]),MSG_CONFIRM,(struct sockaddr*)&P2addr,size);
    					sendto(sfd,direcs[i],strlen(direcs[i]),MSG_CONFIRM,(struct sockaddr*)&P3addr,size);
    					int k;
    					for(k=0;k<3;k++)
    					{
    						if(freePf[k]==0)
    						break;
    					}
    					sprintf(pf,"%d",ports[k]);
    					freePf[k]=1;
    					sendto(tsfd[i],pf,strlen(pf)+1,MSG_CONFIRM,(struct sockaddr*)&incmaddr,size);
    				}
    				else{
    					char noFree[1024]="noSpace";
    					sendto(tsfd[i],noFree,strlen(noFree),MSG_CONFIRM,(struct sockaddr*)&incmaddr,size);
    				}
        		}
        	}
        }
    }
    

}
