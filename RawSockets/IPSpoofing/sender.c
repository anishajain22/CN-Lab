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
	
	struct iphdr *ip=(struct iphdr *)buffer;
	struct udphdr *udp=(struct  udphdr *)(buffer+sizeof(struct iphdr));
	memset(buffer, 0, 8192);

	ip->ihl=5;
	ip->version=4;
	ip->tos=16;
	ip->tot_len=sizeof(struct iphdr)+sizeof(struct udphdr);//+sizeof(mymesg.msg);
	ip->id=htons(54321);
	ip->ttl=64;
	ip->protocol=17;
	ip->saddr=inet_addr(send_addr);
	ip->daddr=inet_addr(recv_addr);
	
	udp->source=htons(8080);
	udp->dest=htons(9090);
	udp->len=htons(sizeof(struct udphdr));	
	ip->check = csum((unsigned short *)buffer,sizeof(struct iphdr) + sizeof(struct udphdr));
	
	int count=0;
	
	while(count<10){
		if (sendto(rsfd, buffer, ip->tot_len, 0,(struct sockaddr *)&foreignaddr, sizeof(foreignaddr)) < 0){
		perror("sendto()");
		exit(3);
		}
		
		printf("OK: one packet is sent.\n");
		count++;
	}

	//close(rsfd);
	return 0;
}


