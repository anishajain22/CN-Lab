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
#include<pthread.h>
#define SOCKPATH "./udsocket1"


ssize_t sock_fd_read(int sock, void *buf, ssize_t bufsize, int *fd)
{
    ssize_t     size;

    if (fd) {
        struct msghdr   msg;
        struct iovec    iov;
        union {
            struct cmsghdr  cmsghdr;
            char        control[CMSG_SPACE(sizeof (int))];
        } cmsgu;
        struct cmsghdr  *cmsg;

        iov.iov_base = buf;
        iov.iov_len = bufsize;

        msg.msg_name = NULL;
        msg.msg_namelen = 0;
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_control = cmsgu.control;
        msg.msg_controllen = sizeof(cmsgu.control);
        size = recvmsg(sock,&msg,MSG_CMSG_CLOEXEC);
        if (size < 0) {
            perror ("recvmsg");
            exit(1);
        }
        cmsg = CMSG_FIRSTHDR(&msg);
        if (cmsg && cmsg->cmsg_len == CMSG_LEN(sizeof(int))) {
            if (cmsg->cmsg_level != SOL_SOCKET) {
                fprintf (stderr, "invalid cmsg_level %d\n",
                     cmsg->cmsg_level);
                exit(1);
            }
            if (cmsg->cmsg_type != SCM_RIGHTS) {
                fprintf (stderr, "invalid cmsg_type %d\n",
                     cmsg->cmsg_type);
                exit(1);
            }

            *fd = *((int *) CMSG_DATA(cmsg));
            printf ("received fd %d\n", *fd);
        } else
            *fd = -1;
    } else {
        size = read (sock, buf, bufsize);
        if (size < 0) {
            perror("read");
            exit(1);
        }
    }
    return size;
}

void* commfunc(void* varg)
{
	int nsfd=*(int*) varg;
	char buff[1024];
	while(1){
		sleep(3);
		int bytes=recv(nsfd,buff,1024,0);
		buff[bytes]='\0';
		printf("got %d bytes\n",bytes);
		printf("received %s\n",buff);
		int length=sizeof(buff);
		char tosend[1024];
		sprintf(tosend,"%d",length);
		printf("Sending: %s\n",tosend);
		send(nsfd,tosend,sizeof(tosend),0);
	}
	
}

int main(){
	int ufd,len,t;
	struct sockaddr_un local,remote;
	char str[100];

	if ((ufd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, SOCKPATH);
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	if (bind(ufd, (struct sockaddr *)&local, len) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(ufd, 5) == -1) {
		perror("listen");
		exit(1);
	}
	
	int nufd=accept(ufd,(struct sockaddr *)&remote, &t);
	struct pollfd fds[1];
	fds[0].fd=nufd;
	fds[0].events=POLLIN;
	
	
	
	while(1){
		if(poll(fds,2,2000)>0){
			if(fds[0].revents&POLLIN){
				//read from the fds[0].fd and passs  to thread as arguement
				char buff[1024];
				int fd;
				sock_fd_read(fds[0].fd,buff,sizeof(buff),&fd);
				pthread_t pid;
				pthread_create(&pid,NULL,&commfunc,(void*)&fd);
			}
		}
	}
	
	


}
