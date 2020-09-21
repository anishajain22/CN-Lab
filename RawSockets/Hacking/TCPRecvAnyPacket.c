#include"myHead.h"

int main()
{
    int rsfd;
    if ((rsfd = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 0) 
    {
        perror ("socket() failed ");
        exit (EXIT_FAILURE);
    }
    struct sockaddr_ll device;
    memset (&device, 0, sizeof (device));
    device.sll_family = PF_PACKET;
    int len=sizeof(device);
    while(1)
    {
        char buff[8192];
        while(recvfrom(rsfd,buff,8192,0,(struct sockaddr*)&device,&len)>0)
        {
            struct ethhdr eth=*(struct ethhdr *)(buff);
            
            //printf("\n");
            struct iphdr hdrip = *(struct iphdr *)( buff + sizeof(struct ethhdr) );
            	
			char ipMtch[]="192.168.6.134";
			char* destAddr=inet_ntoa(*((struct in_addr*)&hdrip.daddr));
			if(strcmp(ipMtch,destAddr)==0){
				printf("received for him\n");
				printf("Source: ");
	        	for(int i=0;i<6;i++)
		    	printf("%02x:",eth.h_source[i]);
		        printf("\n");
				printf("----------------------------------------------\n");
				printf("| version: %d | TOS: %d | Length: %d |\n",hdrip.version,hdrip.tos,hdrip.tot_len);
				printf("| ID: %d | FragmentOffset : %d\n",hdrip.id, hdrip.frag_off);
				printf("| TTL: %d | ChckSum: %d |\n",hdrip.ttl, hdrip.check);
				printf("| srcIP: %s |\n",inet_ntoa(*((struct in_addr*)&hdrip.saddr)));
				printf("| destIP : %s |\n",inet_ntoa(*((struct in_addr*)&hdrip.daddr)));
				printf("----------------------------------------------\n");
		        if(hdrip.protocol==IPPROTO_TCP )
		        {
					struct tcphdr hdrtcp=*(struct tcphdr*)(buff + sizeof(hdrip)+ sizeof(struct ethhdr));
					printf("| srcPort: %d | dest: %d | \n", ntohs(hdrtcp.source),ntohs(hdrtcp.dest));
					printf("| seqno: %d |\n", hdrtcp.seq);
					printf("| ackno: %d |\n", hdrtcp.ack_seq);
					printf("| hdrlen: %d | urg %d | ack : %d | psh : %d | reset : %d | syn : %d | fin : %d |\n", hdrtcp.doff, hdrtcp.urg, hdrtcp.ack, hdrtcp.psh, hdrtcp.rst, hdrtcp.syn, hdrtcp.fin);
					
					
			printf("TCP raw socket: payload: %s\n\n", buff+sizeof(hdrip)+sizeof(hdrtcp));
		            
		            if(hdrtcp.source!=25){
		                //forward(buff);
		            }
		        }
				else if(hdrip.protocol==IPPROTO_UDP ){
					struct udphdr hdrudp=*(struct udphdr*)(buff + sizeof(hdrip)+ sizeof(struct ethhdr));
					printf("| src port: %d | dest port: %d | \n", ntohs(hdrudp.source), ntohs(hdrudp.dest));
					printf("| length: %d | chksum: %d |\n",ntohs(hdrudp.len),ntohs(hdrudp.check));
					printf("payload: %s\n\n", buff+sizeof(hdrip)+sizeof(hdrudp));
				}
			}
        }
    }
    return 0;
}
