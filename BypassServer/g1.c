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
	char send_data[1024],recv_data[1024];
	struct hostent *host;
	struct sockaddr_in server_addr,g2,my_addr;  
	int sn_size;
	host = gethostbyname("127.0.0.1");

	if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
	perror("Socket");
	exit(1);
	}
	
	my_addr.sin_family = AF_INET;     
	my_addr.sin_port = htons(6001);   
	my_addr.sin_addr.s_addr = INADDR_ANY; 

	if (bind(sfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))!=0)   
	{   
	    perror("bind failed");   
	    exit(EXIT_FAILURE);   
	}

	server_addr.sin_family = AF_INET;     
	server_addr.sin_port = htons(5000);   
	server_addr.sin_addr.s_addr = INADDR_ANY;

	printf("%d\n",sfd);
	printf("Enter the process you want ot coneect with\n");
	char a[20];
	scanf("%s",a);
	
	sendto(sfd,a,strlen(a),MSG_CONFIRM,(const struct sockaddr *) &server_addr, 
			sizeof(server_addr));
	close(sfd);
	

	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;     
	my_addr.sin_port = htons(6001);   
	my_addr.sin_addr.s_addr = INADDR_ANY; 

	if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	perror("Socket");
	exit(1);
	}

	if (bind(cfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in))<0)   
	{   
	    perror("bind failed");   
	    exit(EXIT_FAILURE);   
	}

	if (listen(cfd, 5) == -1) {
        perror("Listen");
        exit(1);
	}
	

	int fd=accept(cfd, (struct sockaddr *)&g2,&sn_size);
	if(fd>0)
	{
		bytes_recieved = recv(fd,recv_data,1024,0);
		printf("%s\n",recv_data);
	}
	
	return 0;
}
