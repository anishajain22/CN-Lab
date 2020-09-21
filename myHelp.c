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
#include <string.h>   
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   
#include <arpa/inet.h>   
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



-------------------SEMAPHORES----------------------
void wait(int semid)
{
  struct sembuf sb;
  sb.sem_num=0;
  sb.sem_op=-1;
  sb.sem_flg=0;
  semop(semid, &sb, 1);
}

void signal(int semid)
{
  struct sembuf sb;
  sb.sem_num=0;
  sb.sem_op=1;
  sb.sem_flg=0;
  semop(semid, &sb, 1);
}
	
	int sem1,sem2;
	
	sem1 = semget(IPC_PRIVATE,1,0666|IPC_CREAT);
	sem2 = semget(IPC_PRIVATE,1,0666|IPC_CREAT);
		
	semctl(sem1,0,SETVAL,0);
	semctl(sem2,0,SETVAL,0);
	

	//wait and signal respective semaphore


-------------------SHARED MEMORY----------------------

	int shmid1, shmid2;
	
	int* x;
	int* y;
	
	shmid1=shmget(IPC_PRIVATE,sizeof(int),0666|IPC_CREAT);
	shmid2=shmget(IPC_PRIVATE,sizeof(int),0666|IPC_CREAT);
	
	x=(int*)shmat(shmid1,NULL,0);
	y=(int*)shmat(shmid2,NULL,0);


-------------------PTHREAD----------------------

void* func(void* arg){
	int fd=*(int*) arg;
}

	pthread_t pid;
	pthread_create(&pid,NULL,&func,(void*)&arg);


-------------------FILES----------------------

	int fd=open("filename",O_RDWR | O_APPEND | O_CREAT);
	
	read(fd,buff,1024);
	write(fd,buff,1024);


-------------------MSG QUEUES----------------------

	struct msgbuf{
		long type;
		int processno;
		char msg[200];
	}msg;

	//key for anything

	key_t k=ftok("msgfile",65);
	msgid=msgget(k,0666|IPC_CREAT);

	msgsend(msgid,msg,sizeof(msg),0);
	msgrecv(msgid,msg,sizeof(msg),type,0);


-------------------POLL----------------------
	struct pollfd fds[2];

	fds[0].fd=0;
	fds[0].events=POLLIN;

	while(1){

		if(poll(fds,2,2000)>0){

			if(fds[0].revents&POLLIN){

			}
		}	
	}

-------------------SELECT----------------------

	fd_set rset,wset,eset;
	FD_ZERO(&rset);
	FD_ZERO(&wset);
	FD_ZERO(&eset);
	
	while(1){
		FD_SET(fd,&rset);
		FD_SET(fd,&wset);
		FD_SET(fd,&eset);
		
		//calculate maxfd in all these
		if(select(maxfd,&rset,&wset,&eset,NULL)>0){
			
			if(FD_ISSET(fd,&rset)){
				
			}
		}
		
		
	}
	
-------------------SIGNAL _ SIGACTION----------------------
	
void hdfn (int sig, siginfo_t *siginfo, void *context)
{
	printf ("Sending PID: %ld, UID: %ld\n",(long)siginfo->si_pid, (long)siginfo->si_uid);
}
	
	//MAIN FUNC _ SERVER
	struct sigaction act;
 
	memset (&act, '\0', sizeof(act));
 
	act.sa_sigaction = &hdfn;
	act.sa_flags = SA_SIGINFO;
 
	sigaction(SIGUSR1, &act, NULL);
 	
 	
	//MAIN FUNC _ SENDER
	system("pgrep P1 > PID.txt");
	char buffer[20];
	
	fscanf(fopen("PID.txt","r"),"%s",buffer);
	int pid=atoi(buffer);
	printf("%d\n",pid);
	kill(pid,SIGUSR1);

-------------------INET SOCKETS - TCP----------------------
	
	//MAIN FUNC _ CLIENT
	int sfd, bytes_recieved;  
	char send_data[1024],recv_data[1024];
	struct sockaddr_in server_addr;  

	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	perror("Socket");
	exit(1);
	}

	server_addr.sin_family = AF_INET;     
	server_addr.sin_port = htons(5000);   
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero),8); 

	if (connect(sfd, (struct sockaddr *)&server_addr,
	sizeof(struct sockaddr)) == -1) 
	{
	perror("Connect");
	exit(1);
	}
	
	send(sfd,send_data,strlen(send_data), 0);
	recv(sfd,recv_data,1024,0);

	//MAIN FUNC _ SERVER
	int sfd, bytes_recieved;  
	char send_data[1024],recv_data[1024];
	struct sockaddr_in server_addr,taddr;  
	int sn_size=sizeof(server_addr);	
	
	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	perror("Socket");
	exit(1);
	}

	server_addr.sin_family = AF_INET;     
	server_addr.sin_port = htons(6001);   
	server_addr.sin_addr.s_addr = INADDR_ANY;
 
	int d=bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	printf("%d\n",d);

	if (listen(sfd, 5) == -1) {
            perror("Listen");
            exit(1);
	}

	if((nsfd=accept(sfd,(struct sockaddr *)&taddr,&sn_size))==-1){
		perror("Accept\n");
	}

	send(nsfd,send_data,strlen(send_data), 0);
	recv(nsfd,recv_data,1024,0);



-------------------INET SOCKETS - UDP-------------------

	//MAIN FUNC _ SERVER _ CLIENT
	int sockfd; 
	char buffer[MAXLINE]; 
	struct sockaddr_in clientaddr, servaddr; 

	
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	

	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	

	clientaddr.sin_family = AF_INET; 
	clientaddr.sin_port = htons(7000); 
	clientaddr.sin_addr.s_addr = INADDR_ANY; 
	
	int n, len; 
	
	if(bind(sockfd, (struct sockaddr*) &clientaddr,sizeof(struct sockaddr))!=0)
		printf("Error\n");

	sendto(sockfd, (const char *)hello, strlen(hello), 
		MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
			sizeof(servaddr)); 
	printf("Hello message sent.\n"); 
	
	n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
				MSG_WAITALL, (struct sockaddr *) &servaddr, 
				&len); 


-------------------UNIX SOCKETS----------------------

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
        size = recvmsg(sock,&msg,0);
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

	//MAIN FUNC _ CLIENT
	int s, t, len, fd, conn;
    struct sockaddr_un remote;
    char str[100];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
	perror("socket");
	exit(1);
    }
	remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, SOCKPATH);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (connect(s, (struct sockaddr *)&remote, len) == -1) {
	perror("connect");
	exit(1);
    }
    
    sock_fd_write(s,buff,sizeof(buff),fd);
	sock_fd_read(s,buff,sizeof(buff),&fd);

	
	//MAIN FUNC _ SERVER
	int s, s2, t, len;
	struct sockaddr_un local, remote;
	char str[100];

	if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, SOCKPATH);
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	if (bind(s, (struct sockaddr *)&local, len) == -1) {
		perror("bind");
		exit(1);
	}
	if (bind(s, (struct sockaddr *)&local, len) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(s, 5) == -1) {
		perror("listen");
		exit(1);
	}


	if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1) {
		perror("accept");
		exit(1);
	}
	
    sock_fd_read(s2,dom,sizeof(dom),&fd);
	sock_fd_write(s2,dom,sizeof(dom),fd);


-------------------IOV BATCH WRITE----------------------


	//along with Unix Socket code

	//reading function
	iov[0].iov_base = foo;
    iov[0].iov_len = sizeof (foo);
    iov[1].iov_base = bar;
    iov[1].iov_len = sizeof (bar);
    iov[2].iov_base = baz;
    iov[2].iov_len = sizeof (baz);

    nr = readv (fd, iov, 3);
    for (i = 0; i < 3; i++)
            printf ("%d: %s", i, (char *) iov[i].iov_base);

	//writing function
	for (i = 0; i < 3; i++) {
            iov[i].iov_base = buf[i];
            iov[i].iov_len = strlen(buf[i]) + 1;
    }
   int nr = writev (fd, iov, 3);




