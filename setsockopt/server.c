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
	int opt=1;
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
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
       
	}
	
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
	
	int fd=accept(sfd ,(struct sockaddr *)&client_addr,&sn_size);

	printf("%d\n",fd);

}
