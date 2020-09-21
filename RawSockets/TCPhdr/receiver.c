#include <sys/types.h>
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
#include <netinet/tcp.h>

#define MAXLINE 4096
#define BUFFSIZE 8192
#define LISTENQ 1024
#define SERV_PORT 9877


int main(int argc, char* argv[]){
	int rawfd, tcpfd;
	struct sockaddr_in rawip, tcpip;
	struct sockaddr_in raddr;
	int rawiplen, tcpiplen, raddrlen;

	char msg[2048]; int msglen;


	struct iphdr hdrip;
	struct tcphdr hdrtcp;

	int iphdrlen = sizeof(hdrip);
	int tcphdrlen = sizeof(hdrtcp);

	if((rawfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0){  
		perror("Raw socket error!"); exit(0);
	}
	int reuse = 1;
	if(setsockopt(rawfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0){
		perror("Raw sockopt error!"); exit(0);
	}
	
	printf("hey\n");
	rawip.sin_family = AF_INET;
	rawip.sin_port = htons(atoi(argv[1]));
	rawip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	rawiplen = sizeof(rawip);

	if(bind(rawfd, (struct sockaddr*)&rawip, rawiplen) < 0){
		perror("Raw bind error!"); exit(0);
	}

	while(1){
		raddrlen = sizeof(raddr);
		msglen = recvfrom(rawfd, msg, 2048, 0, (struct sockaddr*)&raddr, &raddrlen);

		if(msglen <= 0)continue;

		hdrip = *((struct iphdr*)msg);
		hdrtcp = *((struct tcphdr*)(msg + iphdrlen));

		if(hdrtcp.dest != rawip.sin_port)continue;

		printf("Raw socket:\n");
		printf("hl: %d, version: %d, ttl: %d, protocol: %d\n", hdrip.ihl, hdrip.version, hdrip.ttl, hdrip.protocol);
		printf("src: %s\n", inet_ntoa(*((struct in_addr*)&hdrip.saddr)));
		printf("dest: %s\n", inet_ntoa(*((struct in_addr*)&hdrip.daddr)));
		printf("src port: %d, dest port: %d\n", ntohs(hdrtcp.source), ntohs(hdrtcp.dest));
		printf("seq no: %d, ack no: %d\n", hdrtcp.seq, hdrtcp.ack_seq);
		printf("header length: %d\n", hdrtcp.doff);
		printf("syn: %d, rst: %d\n", hdrtcp.syn, hdrtcp.psh);
		printf("ack: %d, urg: %d\n", hdrtcp.ack, hdrtcp.urg);
		
		printf("TCP raw socket: payload: %s\n\n", msg+iphdrlen+tcphdrlen);
		
		
	}
	close(rawfd);

	return 0;
}
