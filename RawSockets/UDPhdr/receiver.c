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

int main(){
	char send_addr[]="192.168.15.1";
	char recv_addr[]="192.168.6.133";
	int rsfd;
	char msg[4096];
	struct sockaddr_in localaddr, foreignaddr;
	int locallen,foreignlen,msglen;
	int one;
	int *val=&one;
	struct iphdr hdrip;
	struct udphdr hdrudp;
	int iphdrlen=sizeof(hdrip);
	int udphdrlen=sizeof(hdrudp);
	
	if((rsfd=socket(PF_INET, SOCK_RAW, IPPROTO_UDP))<0){
		perror("raw socket creation");
	}
	
	if(setsockopt(rsfd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one))){
		perror("setsockopt() error"); exit(-1);
	}
	memset(&localaddr,0,sizeof(localaddr));
	localaddr.sin_family=AF_INET;
	localaddr.sin_addr.s_addr=inet_addr(recv_addr);
	localaddr.sin_port = htons(9002);
	locallen=sizeof(localaddr);
	
	if(bind(rsfd,(struct sockaddr*)&localaddr,locallen)<0){
		perror("bind error");
	}
	
	int count=0;
	char payload[1024];
	while(1){
		foreignlen=sizeof(foreignaddr);
		msglen=recvfrom(rsfd,msg,4096,0, (struct sockaddr*)&foreignaddr, &foreignlen);
		
		if(msglen<=0)continue;
		
		hdrip=*(struct iphdr*)msg;
		
		printf("Count %d\n", count);
		count++;
		
		hdrudp=*(struct  udphdr *)(msg+sizeof(struct iphdr));
		
		strcpy(payload,msg+sizeof(hdrudp)+sizeof(hdrip));

		printf("hl: %d, version: %d, ttl: %d, protocol: %d\n", hdrip.ihl, hdrip.version, hdrip.ttl, hdrip.protocol);
		printf("src: %s\n", inet_ntoa(*((struct in_addr*)&hdrip.saddr)));
		printf("dest: %s\n", inet_ntoa(*((struct in_addr*)&hdrip.daddr)));

		printf("src port: %d, dest port: %d\n", ntohs(hdrudp.source), ntohs(hdrudp.dest));
		printf("payload: %s\n\n", payload);
		printf("\n");
		
		
	}
	
}
