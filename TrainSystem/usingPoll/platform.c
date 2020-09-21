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

int main()
{
	struct sockaddr_in servaddr,myaddr;
	int P1,P2,P3;
	
	myaddr.sin_family = AF_INET;         
    myaddr.sin_port = htons(8001);     
    myaddr.sin_addr.s_addr = INADDR_ANY;
    
    P1=socket(AF_INET, SOCK_DGRAM, 0);
    bind(sfd, (const struct sockaddr *)&myaddr,sizeof(myaddr));
    
    myaddr.sin_port=htons(8002);
    P1=socket(AF_INET, SOCK_DGRAM, 0);
    bind(sfd, (const struct sockaddr *)&myaddr,sizeof(myaddr));
    
    myaddr.sin_port=htons(8003);
    P1=socket(AF_INET, SOCK_DGRAM, 0);
    bind(sfd, (const struct sockaddr *)&myaddr,sizeof(myaddr));
    
    servaddr.sin_family = AF_INET;         
    servaddr.sin_port = htons(5001);     
    servaddr.sin_addr.s_addr = INADDR_ANY;
    
    
    int 
    recvfrom()
    
    
    
    
    
    
}
