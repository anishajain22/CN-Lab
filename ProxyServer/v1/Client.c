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
	struct hostent *host;
	struct sockaddr_in server_addr;  

	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	perror("Socket");
	exit(1);
	}

	server_addr.sin_family = AF_INET;     
	server_addr.sin_port = htons(5000);   
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero),8); 

	if (connect(sfd, (struct sockaddr *)&server_addr,
	sizeof(struct sockaddr)) == -1) 
	{
	perror("Connect");
	exit(1);
	}
	
	printf("Enter the server you want to connect to\n ");
	scanf("%s",send_data);
	send(sfd,send_data,strlen(send_data), 0);
	while(1)
	{

		printf("Send data\t\n");
		scanf("%s",send_data);
		send(sfd,send_data,strlen(send_data), 0);   
		bytes_recieved=recv(sfd,recv_data,1024,0);
		recv_data[bytes_recieved] = '\0';
		printf("Received Data: \t\n%s\n",recv_data);
	}  
	return 0;

}
