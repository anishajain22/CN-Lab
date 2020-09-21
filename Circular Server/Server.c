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
int sfd1,sfd2;
struct mymesg
{
	long mtype;
	int processno;
	char mtext[512];
};
int pids[100]={0};
int npids=0;

int main()
{
	
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;         
	myaddr.sin_port = htons(7000);     
	myaddr.sin_addr.s_addr = INADDR_ANY;
	int cfd=socket(AF_INET, SOCK_STREAM, 0);
	printf("%d\n",cfd);
	if(bind(cfd, (struct sockaddr *)&myaddr, sizeof(myaddr))==-1)
	perror("bind");
	if (listen(cfd, 5) == -1) {
        perror("Listen");
        exit(1);
	}
	int sn_size=sizeof(myaddr);
	int fd=accept(cfd, (struct sockaddr *)&myaddr,&sn_size);
	struct pollfd fds[2];
	fds[0].fd=0;
	fds[0].events=POLLIN;
	
	
	fds[1].fd=fd;
	fds[1].events=POLLIN;
	char s[1024];	
	while(1)
	{
		if(poll(fds,2,2000)>0)
		{
			if(fds[0].revents&POLLIN)
			{
				fgets(s,1024,stdin);
				write(fds[1].fd,s,1024);
				
			}
			if(fds[1].revents&POLLIN)
			{
				read(fds[1].fd,s,1024);
				printf("Received: %s\n",s);
			}
		}
	}


}
