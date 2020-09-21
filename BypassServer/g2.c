#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main()
{
	int sfd,cfd, bytes_recieved;  
	char send_data[1024]="hiiii";
	char recv_data[1024];
	struct hostent *host;
	struct sockaddr_in my_addr,servaddr,g1addr;  

	host = gethostbyname("127.0.0.1");

	if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
	perror("Socket");
	exit(1);
	}
	if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	perror("Socket");
	exit(1);
	}
	
	my_addr.sin_family = AF_INET;     
	my_addr.sin_port = htons(7001);   
	my_addr.sin_addr .s_addr = INADDR_ANY; 
	if (bind(sfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))<0)   
	{   
	    perror("bind failed");   
	    exit(EXIT_FAILURE);   
	}
	printf("my address %s , %d\n",inet_ntoa(my_addr.sin_addr),ntohs(my_addr.sin_port));
	
	char g1ipaddr[2048];
	char g1portnum[1024];
	int len;

	recvfrom(sfd, (char *)g1ipaddr, 2048, 
				MSG_WAITALL, (struct sockaddr *) &servaddr, 
				&len);
	recvfrom(sfd, (char *)g1portnum, 1024, 
				MSG_WAITALL, (struct sockaddr *) &servaddr, 
				&len);
	printf("group 1 address %s %d\n", g1ipaddr, atoi(g1portnum));
		
	g1addr.sin_family = AF_INET;     
	g1addr.sin_port = htons(atoi(g1portnum)); 
	inet_pton(AF_INET, g1ipaddr, &(g1addr.sin_addr)); 
		
	if (connect(cfd, (struct sockaddr *)&g1addr,
	sizeof(struct sockaddr)) == -1) 
	{
	perror("Connect");
	exit(1);
	}
	send(cfd,send_data,strlen(send_data), 0); 
	
				
	
}
