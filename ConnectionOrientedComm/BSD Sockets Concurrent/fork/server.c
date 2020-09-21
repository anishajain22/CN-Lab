#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void sendrcvfunc(int nsfd)
{		
	char send_data [1024] , recv_data[1024]; 
	int bytes_recieved;
	while(1)
	{
		
		printf("\n SEND (q or Q to quit) %d : ",getpid());
		fgets(send_data,1024*sizeof(char),stdin);

		if (strcmp(send_data , "q") == 0 || strcmp(send_data , "Q") == 0)
		{
			send(nsfd, send_data,strlen(send_data), 0); 
			close(nsfd);
			break;
		}

		else
			send(nsfd, send_data,strlen(send_data), 0);  
		
		bytes_recieved = recv(nsfd,recv_data,1024,0);
		if(bytes_recieved>0)
		{
			recv_data[bytes_recieved] = '\0';

			if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
			{
				close(nsfd);
				break;
			}

			else 
				printf("\n RECIEVED DATA = %s " , recv_data);
				fflush(stdout);
		}
	}
}
void func(int sfd)
{
	int nsfd;
	struct sockaddr_in client_addr;
	int sn_size= sizeof(struct sockaddr);
	if((nsfd=accept(sfd, (struct sockaddr *)&client_addr,&sn_size))>0)
	{
		printf("\n I got a connection from (%s , %d) %d",
	   inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port), nsfd);
		int c=fork();
		if(c>0)
		{	
			sleep(3);
			func(sfd);
		}
		else
		{
			sendrcvfunc(nsfd);
		}
			
	}
	
}
int main()
{
        int sfd, nsfd, bytes_recieved , true = 1;  
        char send_data [1024] , recv_data[1024];       

        struct sockaddr_in server_addr,client_addr;    
        int sin_size;
        
        if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }

        if (setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
            perror("Setsockopt");
            exit(1);
        }
        
        server_addr.sin_family = AF_INET;         
        server_addr.sin_port = htons(5000);     
        server_addr.sin_addr.s_addr = INADDR_ANY; 
        bzero(&(server_addr.sin_zero),8); 

        if (bind(sfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
                                                                       == -1) {
            perror("Unable to bind");
            exit(1);
        }

        if (listen(sfd, 5) == -1) {
            perror("Listen");
            exit(1);
        }
		
	printf("\nTCPServer Waiting for client on port 5000");
        fflush(stdout);


       func(sfd);
} 
