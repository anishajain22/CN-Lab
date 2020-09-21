/*
struct ipheader {
	unsigned char      iph_ihl:5, iph_ver:4;
	unsigned char      iph_tos;
	unsigned short int iph_len;
	unsigned short int iph_ident;
	unsigned char      iph_flag;
	unsigned short int iph_offset;
	unsigned char      iph_ttl;
	unsigned char      iph_protocol;
	unsigned short int iph_chksum;
	unsigned int       iph_sourceip;
	unsigned int       iph_destip;
};
*/
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

int main(){
	char send_addr[]="192.168.15.133";
	char recv_addr[]="192.168.6.133";
	int rsfd;
	struct mymesg mesg;
	struct sockaddr_in localaddr, foreignaddr,senderaddr;
	int locallen,foreignlen,msglen,senderlen;
	int one;
	int *val=&one;
	struct iphdr hdrip;
	struct udphdr hdrudp;
	char *msg;
	int iphdrlen=sizeof(hdrip);
	
	if((rsfd=socket(PF_INET, SOCK_RAW, IPPROTO_UDP))<0){
		perror("raw socket creation");
	}
	
	if(setsockopt(rsfd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one))){
		perror("setsockopt() error"); exit(-1);
	}
	memset(&localaddr,0,sizeof(localaddr));
	localaddr.sin_family=AF_INET;
	localaddr.sin_addr.s_addr=inet_addr(recv_addr);
	localaddr.sin_port=htons(9090);
	locallen=sizeof(localaddr);
	
	memset(&foreignaddr,0,sizeof(foreignaddr));
	foreignaddr.sin_family=AF_INET;
	foreignaddr.sin_addr.s_addr=inet_addr(recv_addr);
	foreignaddr.sin_port=htons(9091);
	foreignlen=sizeof(foreignaddr);


	if(bind(rsfd,(struct sockaddr*)&localaddr,locallen)<0){
		perror("bind error");
	}
	
	int count=0;
	msglen=recvfrom(rsfd,&mesg,sizeof(struct mymesg),0, (struct sockaddr*)&senderaddr, &senderlen);
	
	printf("hl: %d, version: %d, ttl: %d, protocol: %d\n", mesg.hdrip.ihl, mesg.hdrip.version, mesg.hdrip.ttl, mesg.hdrip.protocol);
	
	struct mymesg sendmesg;
	sendmesg.hdrip=mesg.hdrip;
	sendmesg.hdrudp=mesg.hdrudp;
	struct in_addr addr;
	addr.s_addr=mesg.hdrip.saddr;
	printf("src: %s\n", inet_ntoa(addr));
	memset(sendmesg.msg,0, 1024);
	memset(sendmesg.options,0, 1024);
	
	sprintf(sendmesg.options,"%s",inet_ntoa(addr));
	sprintf(sendmesg.msg,"%s",mesg.msg);
	addr.s_addr=mesg.hdrip.daddr;
	printf("dest: %s\n", inet_ntoa(addr));
	printf("msg sent: %s\n",sendmesg.msg);
	
	sendmesg.hdrip.saddr=mesg.hdrip.daddr;
	sendmesg.hdrudp.source=mesg.hdrudp.dest;
	sendmesg.hdrudp.dest=htons(9091);
	
	sleep(5);
	printf("msg to be sent: %s\n",sendmesg.msg);
	printf("tracer: %s\n",sendmesg.options);
	msglen=sendto(rsfd,&sendmesg,sizeof(sendmesg),0,(struct sockaddr*)&foreignaddr,sizeof(foreignaddr));
	
}
