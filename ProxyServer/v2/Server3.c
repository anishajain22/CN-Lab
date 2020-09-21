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
	server_addr.sin_port = htons(6003);   
	server_addr.sin_addr.s_addr = INADDR_ANY;
 
	int d=bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	printf("%d\n",d);

	if (listen(sfd, 5) == -1) {
            perror("Listen");
            exit(1);
        }

	int nsfd=accept(sfd,(struct sockaddr *)&taddr,&sn_size);
	printf("Accepting connection %d\n",nsfd);
	
	struct pollfd fds[1];
	fds[0].fd=nsfd;
	fds[0].events=POLLIN;
	

	while(1)
	{
		if(poll(fds,1,2000)>0)
		{
			if(fds[0].revents&POLLIN)
			{
				fflush(stdout);		
				bytes_recieved=recv(fds[0].fd,recv_data,1024,0);
				recv_data[bytes_recieved] = '\0';
				printf("Received Data: \t\n%s\n",recv_data);

				printf("Enter data to send to client\n");
				scanf("%s",send_data);
				int d=send(fds[0].fd,send_data,strlen(send_data), 0);
				printf("%d\n",d);
			}
		}
		
	}	
	return 0;

}
