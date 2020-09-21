#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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

int main()
{
	int sfd,nsfd,bytes_recieved , true = 1;  
    char send_data [1024] , recv_data[1024];       

    struct sockaddr_in server_addr,client_addr;    
    int sn_size;
    
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }
	fd_set readfds; 
    
    server_addr.sin_family = AF_INET;         
    server_addr.sin_port = htons(5000);     
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    bzero(&(server_addr.sin_zero),8); 
    
    if (bind(sfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))<0)   
	{   
	    perror("bind failed");   
	    exit(EXIT_FAILURE);   
	} 
	
	struct pollfd fds[10];
	int nsfds=1;
	fds[0].fd=sfd;
	fds[0].events=POLLIN;
	if (listen(fds[0].fd, 5) == -1) {
        perror("Listen");
        exit(1);
	}
	
	while(1)
	{
		if(poll(fds, nsfds, 2000)>0)
		{
			//printf("got something\n");
			if(fds[0].revents&POLLIN)
			{
				printf("accept\n");
				fflush(stdout);
				int fd=accept(fds[0].fd, (struct sockaddr *)&client_addr,&sn_size);
				printf("%d\n",fd);
				if(fd>0){
				fds[nsfds].fd=fd;
				fds[nsfds].events=POLLIN;
				printf("after\n");
				fflush(stdout);
				nsfds++;
				getpeername(nsfd,(struct sockaddr*)&client_addr,&sn_size);
				printf("\n I got a connection from (%s , %d)",
	   inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
				fflush(stdout);
				}
			}
			for(int i=1;i<nsfds;i++)
			{	printf("in here\n");
				if(fds[i].fd!=-1 && fds[i].revents&POLLIN)
				{
					int r;
					ioctl(fds[i].fd, FIONREAD,&r);
					if(r>0)
					{
					printf("received here %d\n",i);
					bytes_recieved = recv(fds[i].fd,recv_data,1024,0);
					recv_data[bytes_recieved]='\0';
					strcpy(send_data,recv_data);
					fflush(stdout);
					printf("Received : \t\n%s\n",recv_data);
					send(fds[i].fd, send_data,strlen(send_data), 0);
					fflush(stdout);
					}
					else
					{
						close(fds[i].fd);
						fds[i].fd=-1;
					}
					
				}
			}
		
		}
	}

}
