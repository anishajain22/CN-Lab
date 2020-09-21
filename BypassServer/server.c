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
char*  hostnames[4];
int ports[4]={7001,7002,7003,7004};
int main()
{
	int sfd,nsfd,bytes_recieved , true = 1;  
    char send_data [1024] , recv_data[1024];  
    struct hostent *host;     
	host = gethostbyname("127.0.0.1");
    struct sockaddr_in server_addr,client_addr;    
    int sn_size;
    
    if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }
	fd_set readfds; 
    
    server_addr.sin_family = AF_INET;         
    server_addr.sin_port = htons(5000);     
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    if ( bind(sfd, (const struct sockaddr *)&server_addr, 
			sizeof(server_addr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
    printf("%s , %d\n",inet_ntoa(server_addr.sin_addr),ntohs(server_addr.sin_port));
	
	struct pollfd fds[1];
	fds[0].fd=sfd;
	fds[0].events=POLLIN;
	
	while(1)
	{
		if(poll(fds,1,2000)>0)
		{
			if(fds[0].revents&POLLIN)
			{	
				printf("here\n");
				struct sockaddr_in g1;
				int size=sizeof(struct sockaddr_in);
				char buffer[20];
				int d=recvfrom(fds[0].fd,buffer,20,MSG_WAITALL,(struct sockaddr*) &g1,&size);				
				

				char addr[2048];

				strcpy(addr,inet_ntoa(g1.sin_addr));
				printf("%d\n", ntohs(g1.sin_port));

				int portnum=ntohs(g1.sin_port);
				printf("portnum %d\n", portnum);
				char portstring[1024];
				
				sprintf(portstring,"%d",portnum);
				//printf(" %s %s\n", addr, portstring);
				//printf("%s , %d\n",inet_ntoa(g2.sin_addr),ntohs(g2.sin_port));
				
				struct sockaddr_in g2;
				
				g2.sin_addr.s_addr = INADDR_ANY;
				g2.sin_port=htons(7001);

				d=sendto(sfd,addr,strlen(addr),MSG_CONFIRM,(const struct sockaddr*) &g2,size) ;
				printf("%d\n",d);
				d=sendto(sfd,portstring,strlen(portstring),MSG_CONFIRM,(const struct sockaddr*) &g2,size);
				printf("%d\n",d);
			}
		}
	}
	
}
