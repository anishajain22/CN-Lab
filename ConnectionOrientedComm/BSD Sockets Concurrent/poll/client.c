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

	int sfd, bytes_recieved;  
	char send_data[1024],recv_data[1024];
	struct hostent *host;
	struct sockaddr_in server_addr;  

	host = gethostbyname("127.0.0.1");

	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	perror("Socket");
	exit(1);
	}

	server_addr.sin_family = AF_INET;     
	server_addr.sin_port = htons(5000);   
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8); 

	if(inet_pton(AF_INET,"127.0.0.1", &server_addr.sin_addr)<=0)
	{
		printf("fv\n");
		return -1;	
	}
	if (connect(sfd, (struct sockaddr *)&server_addr,
	sizeof(struct sockaddr)) == -1) 
	{
	perror("Connect");
	exit(1);
	}
	int sn_size;
	struct sockaddr_in srv;
	int sz;
	int d=getsockname(sfd,(struct sockaddr*)&srv,&sz);
	printf("%d\n",d);
	printf("Self IP Addr %s Port %d\n",inet_ntoa(srv.sin_addr),ntohs(srv.sin_port));
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
