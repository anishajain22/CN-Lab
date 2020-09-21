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

int main()
{
	system("gcc Server1.c -o S1");
	system("gcc Server1.c -o S2");
	system("gcc Server1.c -o S3");
	system("gcc Server1.c -o S4");
	system("gcc Server1.c -o S5");
	system("gcc -pthread Proxy.c -o P");
	system("gcc Client.c -o C1");
	system("gcc Client.c -o C2");
	system("gnome-terminal ./S1");
	/*system("gnome-terminal ./S2");
	system("gnome-terminal ./S3");
	system("gnome-terminal ./S4");
	system("gnome-terminal ./S5");*/

}
