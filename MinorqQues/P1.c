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

int main()
{
	int fd=fileno(popen("./P6","w"));
	dup2(fd,1);
	int c;
	char* args[]={NULL};
	c=fork();
	if(c==0)
	execv("./P2",args);
	c=fork();
	if(c==0)
	execv("./P3",args);
	while(1);
}
