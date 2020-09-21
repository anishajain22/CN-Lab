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

int main()
{
	int sfd, bytes_recieved;  
	char send_data[1024],recv_data[1024];
	struct sockaddr_in server_addr,taddr;  
	int sn_size=sizeof(server_addr);	
	
	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	perror("Socket");
	exit(1);
	}

	server_addr.sin_family = AF_INET;     
	server_addr.sin_port = htons(6001);   
	server_addr.sin_addr.s_addr = INADDR_ANY;
 
	int d=bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	printf("%d\n",d);

	if (listen(sfd, 5) == -1) {
            perror("Listen");
            exit(1);
        }
	
	struct pollfd fds[100];
	fds[0].fd=0;
	fds[0].events=POLLIN;
	
	fds[1].fd=sfd;
	fds[1].events=POLLIN;
	int count=2;

	while(1)
	{
		if(poll(fds,100,2000)>0)
		{
			if(fds[0].revents&POLLIN)
			{
				printf("Which client they wish to connect to\n");
				int d;
				scanf("%d",&d);
				printf("Enter data to send to client\n");
				scanf("%s",send_data);
				d=send(fds[d+1].fd,send_data,strlen(send_data), 0);
				printf("%d\n",d);
				
				
			}
			if(fds[1].revents&POLLIN)
			{
				fds[count].fd=accept(fds[1].fd,(struct sockaddr *)&taddr,&sn_size);
				printf("connected to %d client\n",count-1);
				count++;
			}
			
			for(int i=2;i<count;i++)
			{
				if(fds[i].revents&POLLIN)
				{
					fflush(stdout);		
					bytes_recieved=recv(fds[0].fd,recv_data,1024,0);
					recv_data[bytes_recieved] = '\0';
					printf("Received Data: \t\n%s\n",recv_data);
				}
			}
			
		}
		
	}	
	return 0;

}
