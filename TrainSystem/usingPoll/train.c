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
int cfd;
void hdfn()
{
	char gone[2]="BY";
	printf("leavinf station\n");
	int d=send(cfd,gone,strlen(gone),0);
	printf("%d\n",d);
	raise(SIGKILL);
}
int main()
{
	printf("Enter direction you are coming from\n");
	char c;
	scanf("%c",&c);
	int portno;
	if(c=='V')
	portno=7001;
	else if(c=='H')
	portno=7002;
	else if(c=='D')
	portno=7003;
	
	struct sockaddr_in servaddr;
	
	servaddr.sin_family = AF_INET;         
    servaddr.sin_port = htons(portno);     
    servaddr.sin_addr.s_addr = INADDR_ANY;
	
	int size=sizeof(servaddr);
	
	int sfd = socket(AF_INET, SOCK_DGRAM, 0);
	char come[20]="come";
	
	signal(SIGINT,hdfn);
	
	sendto(sfd, come, strlen(come), MSG_CONFIRM, (struct sockaddr*)&servaddr,size);
	
	char port[1024];
	
	int d=recvfrom(sfd,port, 1024, MSG_WAITALL,(struct sockaddr*)&servaddr,& size);
	port[d]='\0';
	if(strcmp(port,"noSpace")==0)
	{
		printf("no empty platform\n");
		return 0;
	}
	portno =atoi(port);
	
	cfd=socket(AF_INET, SOCK_STREAM, 0);
	printf("to connect with port %d\n",portno);
	servaddr.sin_port=htons(portno);
	char details[1024]="HI";
	
	if(connect(cfd, (struct sockaddr*)&servaddr, sizeof(servaddr))!=-1)
	{
		printf("connected with pf\n");
		send(cfd,details, strlen(details), 0);
	}
	while(1);
	
}
