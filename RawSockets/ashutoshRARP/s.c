#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>       // IPPROTO_RAW, INET_ADDRSTRLEN
#include <netinet/ip.h>       // IP_MAXPACKET (which is 65535)
#include <net/if.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>

struct arphdr
{
    uint16_t htype;		//Hardware type
    uint16_t ptype;		//Protocol type
    uint8_t hlen;		//Hardware address length
    uint8_t plen;		//Protocol address lengt
  	uint16_t opcode;	//Operation code
    uint8_t sender_mac[6];  
    uint8_t sender_ip[4];
    uint8_t target_mac[6];
	uint8_t target_ip[4]; 
};
int main()
{
  int rsfd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_RARP));
	char  interface[40];
	strcpy (interface,"ens33");
	if(rsfd==-1)
	{
		 perror("socket");
	}
	char* buf = (char*)malloc(1500);
	uint8_t src[6],dst[6];

	//usr mac address
	//00:0c:29:11:8a:4c
	src[0] = 0x00;
	src[1] = 0x0c;
	src[2] = 0x29;
	src[3] = 0x11;
	src[4] = 0x8a;
	src[5] = 0x4c;

	//local gateway mac address
	//00:50:56:ed:ff:c8
	dst[0] = 0x00;
	dst[1] = 0x50;
	dst[2] = 0x56;
	dst[3] = 0xed;
	dst[4] = 0xff;
	dst[5] = 0xc8;
	
	memcpy(buf,dst,6*(sizeof (uint8_t)));
	memcpy(buf+6*(sizeof (uint8_t)),src,6*(sizeof (uint8_t)));
	
	buf[12] = ETH_P_RARP / 256;
	buf[13] = ETH_P_RARP % 256;
	
	struct arphdr* arp = (struct arphdr*)(buf+14);
	arp->htype = htons(1);		//because we use ethernet
	arp->ptype = 8;	// ETH_P_IP = 0x0800 
	arp->hlen = 6;
	arp->plen = 4;
	arp->opcode = htons(0x01);		// RARP request
	
	memcpy(arp->sender_mac ,src,6*(sizeof(uint8_t)));
	memcpy(arp->target_mac ,src,6*(sizeof(uint8_t)));
	
	// Friend's IP
	//192.168.6.133
	arp->sender_ip[0] = 192;
	arp->sender_ip[1] =168;
	arp->sender_ip[2] = 6;
	arp->sender_ip[3] = 133;
	
	//Gateway IP
	//192.168.6.2
	//arp->target_ip[0] = 192;
	//arp->target_ip[1] = 168;
	//arp->target_ip[2] = 6;
	//arp->target_ip[3] = 2;
	
	memcpy(buf+14,arp,28);
  struct sockaddr_ll device;
  memset (&device, 0, sizeof (device));
	if ((device.sll_ifindex = if_nametoindex (interface)) == 0)
	{
		perror ("if_nametoindex() failed to obtain interface index ");
		exit (EXIT_FAILURE);
	}
	printf ("Index for interface %s is %i\n", interface, device.sll_ifindex);
  device.sll_family = PF_PACKET;
 	memcpy (device.sll_addr, dst, 6 * sizeof (uint8_t));
	device.sll_halen = 6;
	int bytes;
	if ((bytes = sendto (rsfd, buf,42, 0, (struct sockaddr *) &device,sizeof(device)))<= 0){
		perror ("sendto() failed");
		exit (EXIT_FAILURE);
  }
  else printf("%d\n",bytes);
}
