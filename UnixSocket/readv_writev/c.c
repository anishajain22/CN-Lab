#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/uio.h>
#include<sys/stat.h>
#define SOCKET_NAME "abc.socket"
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
int wrv(int fd)
{
	struct iovec iov[3];
        //int nr;
        int i;

        char *buf[] = {
                "The term buccaneer comes from the word boucan.\n",
                "A boucan is a wooden frame used for cooking meat.\n",
                "Buccaneer is the West Indies name for a pirate.\n" };

       /* if (fd == −1) {
                perror ("open");
                return 1;
        }*/

        /* fill out three iovec structures */
        for (i = 0; i < 3; i++) {
                iov[i].iov_base = buf[i];
                iov[i].iov_len = strlen(buf[i]) + 1;
        }
        /* with a single call, write them all out */
        int nr = writev (fd, iov, 3);
        if (nr<0) {
                perror ("writev");
                return 1;
        }
        //printf ("wrote %d bytes\n", nr);

        if (close (fd)) {
                perror ("close");
                return 1;
        }
}
int main()
{
	int usfd;
	int opt=1;
	if((usfd=socket(AF_UNIX, SOCK_STREAM, 0))==-1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	} 
	struct sockaddr_un name;
	memset(&name, 0, sizeof(struct sockaddr_un));
	name.sun_family = AF_UNIX;
        strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);
	
	
	int wfd=open("p.txt",O_WRONLY);
	printf("%d\n",wfd);
	wrv(wfd);
	int rfd=open("p.txt",O_RDONLY);
	printf("%d\n",rfd);
	/*char buff[4];
	read(rfd,buff,4);
	printf("%s\n",buff);*/
	
	if (connect(usfd, (struct sockaddr *)&name, sizeof(name)) < 0) 
    	{ 
        	printf("\nConnection Failed \n"); 
        	return -1; 
    	}
	char buffer[10]=" ";
	printf("%ld",sock_fd_write(usfd,buffer,strlen(buffer),rfd));
	return 0;
}
