#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/select.h>

int main(){
	int sfd;
	sfd=socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in server;
	server.sin_family = AF_INET;         
    server.sin_port = htons(6002);     
    server.sin_addr.s_addr = INADDR_ANY; 
    bzero(&(server.sin_zero),8); 
    int len;
        
    char buff[]="Heythere";
    char got[1024];
    int bytes; 
    if(connect(sfd,(struct sockaddr*)&server,sizeof(struct sockaddr))!=-1){
    	while(1){
    		printf("sending: %s to %d\n",buff, sfd);
    		bytes=send(sfd,buff,sizeof(buff),0);
    		printf("sent %d\n",bytes);
			bytes=recv(sfd,got,1024,0);
			got[bytes]='\0';
			printf("%s\n",got);
    	}
    	
    }

}
