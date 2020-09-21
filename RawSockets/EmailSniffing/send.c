// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT	 8080 
#define MAXLINE 1024 

// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from client"; 
	struct sockaddr_in clientaddr, servaddr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(PF_INET, SOCK_DGRAM, 80)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = inet_addr("192.168.6.133"); 
	
	clientaddr.sin_family = AF_INET; 
	clientaddr.sin_port = htons(7000); 
	clientaddr.sin_addr.s_addr = INADDR_ANY; 

	int n, len; 
	
	if(bind(sockfd, (struct sockaddr*) &clientaddr,sizeof(struct sockaddr))!=0)
		printf("Error\n");

	sendto(sockfd, (const char *)hello, strlen(hello), 
		MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
			sizeof(servaddr)); 
	printf("Hello message sent.\n"); 
	
	while(1);

	return 0; 
} 

