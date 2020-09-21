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
#include<sys/poll.h>
#define SOCKPATH "./udsocket1"

struct service{
	int id;
	struct sockaddr_in addr;
};

ssize_t sock_fd_write(int sock, void *buf, ssize_t buflen, int fd)
{
    ssize_t     size;
    struct msghdr   msg;
    struct iovec    iov;
    union {
        struct cmsghdr  cmsghdr;
        char        control[CMSG_SPACE(sizeof (int))];
    } cmsgu;
    struct cmsghdr  *cmsg;

    iov.iov_base = buf;
    iov.iov_len = buflen;

    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    if (fd != -1) {
        msg.msg_control = cmsgu.control;
        msg.msg_controllen = sizeof(cmsgu.control);

        cmsg = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_len = CMSG_LEN(sizeof (int));
        cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;

        printf ("passing fd %d\n", fd);
        *((int *) CMSG_DATA(cmsg)) = fd;
    } else {
        msg.msg_control = NULL;
        msg.msg_controllen = 0;
        printf ("not passing fd\n");
    }

    size = sendmsg(sock, &msg, 0);

    if (size < 0)
        perror ("sendmsg");
    return size;
}


int main()
{
	int sfd2,nsfd2,ufd;
	int latest=0,len;
	int maxLatest=15;
	
	struct service clients[15];
	if ((ufd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
	struct sockaddr_un remote2;
    remote2.sun_family = AF_UNIX;
    strcpy(remote2.sun_path, SOCKPATH);
    len = strlen(remote2.sun_path) + sizeof(remote2.sun_family);
	if (connect(ufd, (struct sockaddr *)&remote2, len) == -1) {
        perror("connect");
        exit(1);
    }
	
	
	int port2=6002;
	struct sockaddr_in super2,clientaddr;
	super2.sin_family = AF_INET;         
    super2.sin_port = htons(port2);     
    super2.sin_addr.s_addr = INADDR_ANY; 
    bzero(&(super2.sin_zero),8); 
    
    if ((sfd2 = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }
    
    if (bind(sfd2, (struct sockaddr*)&super2, sizeof(struct sockaddr))<0)   
	{   
	    perror("bind failed");   
	    exit(EXIT_FAILURE);   
	}
	
	listen(sfd2,5);
	
	struct pollfd fds[1];
	fds[0].fd=sfd2;
	fds[0].events=POLLIN;
	
	while(1){
		if(poll(fds,1,2000)>0){
			if(fds[0].revents&POLLIN){
				nsfd2=accept(sfd2,(struct sockaddr*)&clientaddr,&len);
				clients[latest].addr=clientaddr;
				clients[latest].id=latest;
				latest++;
				
				//send nsfd2 to serverprocess
				char buff[1024]="Sending nsfd to server2";
				sock_fd_write(ufd,buff,sizeof(buff),nsfd2);
				
				close(nsfd2);
			}
		}
	} 
	
	
}

