#include <sys/types.h>
#include<string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ipc.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
struct mymesg{
	struct iphdr hdrip;
	struct udphdr hdrudp;
	char options[1024];
	char msg[1024];
};

unsigned short csum(unsigned short *buf, int nwords)
{
  unsigned long sum;
  for(sum=0; nwords>0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum &0xffff);
  sum += (sum >> 16);
  return (unsigned short)(~sum);
}

int main()
{
	int rsfd;
	char send_addr[]="192.168.15.1";
	char recv_addr[]="192.168.6.133";
	
	int one = 1;
	const int *val = &one;
	
	if ((rsfd =socket(PF_INET, SOCK_RAW, IPPROTO_UDP))< 0) {
	perror("socket() error");
	exit(2);
	}

	if(setsockopt(rsfd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0) {
	perror("setsockopt() error");
	exit(2);
	}


	struct sockaddr_in foreignaddr;

	foreignaddr.sin_family=AF_INET;
	foreignaddr.sin_port=htons(9090);
	foreignaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	char buffer[8192];
	
	struct mymesg mesg;
	

	mesg.hdrip.ihl=5;
	mesg.hdrip.version=4;
	mesg.hdrip.tos=16;
	mesg.hdrip.tot_len=sizeof(mesg);
	mesg.hdrip.id=htons(54321);
	mesg.hdrip.ttl=64;
	mesg.hdrip.protocol=17;
	mesg.hdrip.saddr=inet_addr(send_addr);
	mesg.hdrip.daddr=inet_addr(recv_addr);
	
	mesg.hdrudp.source=htons(8080);
	mesg.hdrudp.dest=htons(9090);
	mesg.hdrudp.len=htons(sizeof(struct udphdr));	
	mesg.hdrip.check = csum((unsigned short *)buffer,sizeof(struct iphdr) + sizeof(struct udphdr));
	mesg.options[0]='\0';
	strcpy(mesg.msg,"Hey There");
	
	int count=0;
	
	if (sendto(rsfd, &mesg, mesg.hdrip.tot_len, 0,(struct sockaddr *)&foreignaddr, sizeof(foreignaddr)) < 0){
	perror("sendto()");
	exit(3);
	}
	
	printf("OK: one packet is sent.\n");

	//close(rsfd);
	return 0;
}


