#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include<errno.h>
#include<poll.h>
#include <sys/uio.h>
#include<sys/stat.h>
#define SOCKET_NAME "abc.socket"
ssize_t sock_fd_read(int sock, void *buf, ssize_t bufsize, int *fd)
{
    ssize_t     size;

    if (fd) 
   {
        struct msghdr   msg;
        struct iovec    iov;
        union 
	{
            struct cmsghdr  cmsghdr;
            char control[CMSG_SPACE(sizeof (int))];
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
        size = recvmsg(sock,&msg,0);
        if (size < 0) 
	{
            perror ("recvmsg");
            exit(1);
        }
        cmsg = CMSG_FIRSTHDR(&msg);
        if (cmsg && cmsg->cmsg_len == CMSG_LEN(sizeof(int))) 
	{
            if (cmsg->cmsg_level != SOL_SOCKET) 
	    {
                fprintf (stderr, "invalid cmsg_level %d\n",cmsg->cmsg_level);
                exit(1);
            }
            if (cmsg->cmsg_type != SCM_RIGHTS) {
                fprintf (stderr, "invalid cmsg_type %d\n",cmsg->cmsg_type);
                exit(1);
            }
            *fd = *((int *) CMSG_DATA(cmsg));
            printf ("received fd %d\n", *fd);
        }
	else
            *fd = -1;
    } 
   else 
   {
        size =read(sock, buf, bufsize);
        if (size < 0) 
	{
            perror("read");
            exit(1);
        }
    }
    return size;
}
int rdv(int fd)
{
	char foo[48], bar[51], baz[49];
        struct iovec iov[3];
        int nr;
        int i;


        /* set up our iovec structures */
        iov[0].iov_base = foo;
        iov[0].iov_len = sizeof (foo);
        iov[1].iov_base = bar;
        iov[1].iov_len = sizeof (bar);
        iov[2].iov_base = baz;
        iov[2].iov_len = sizeof (baz);

        /* read into the structures with a single call */
        nr = readv (fd, iov, 3);
        /*if (nr == −1) {
                perror ("readv");
                return 1;
        }*/

        for (i = 0; i < 3; i++)
                printf ("%d: %s", i, (char *) iov[i].iov_base);

        if (close (fd)) {
                perror ("close");
                return 1;
        }
}
int main()
{
	int usfd;
	unlink(SOCKET_NAME);
	if((usfd=socket(AF_UNIX, SOCK_STREAM, 0))==-1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	struct sockaddr_un name;
	int len=sizeof(name);
	memset(&name, 0, sizeof(struct sockaddr_un));
	name.sun_family = AF_UNIX;
        strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);
	if(bind(usfd, (const struct sockaddr *) &name,sizeof(struct sockaddr_un))==-1)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}
	
	if(listen(usfd,3)<0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	struct pollfd pfd[1];
	pfd[0].fd=usfd;
	pfd[0].events=POLLIN;
	while(1)
	{
		poll(pfd,1,-1);
		printf("waiting\n");
		if(pfd[0].revents && POLLIN)
		{
			int nsfd=accept(usfd,(struct sockaddr *)&name,(socklen_t*)&len);
			char buff[80]=" ";
			int rfd=1;
			sock_fd_read(nsfd,buff,strlen(buff),&rfd);
			rdv(rfd);
			/*char rr[4];
			read(rfd,rr,5);
			printf("%s\n",rr);*/
		}
	}
	return 0;
}
