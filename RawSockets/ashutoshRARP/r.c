#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>           // close()
#include <string.h>           // strcpy, memset()
#include <netinet/ip.h>       // IP_MAXPACKET (65535)
#include <sys/types.h>        // needed for socket(), uint8_t, uint16_t
#include <sys/socket.h>       // needed for socket()
#include <linux/if_ether.h>   // ETH_P_ARP = 0x0806, ETH_P_ALL = 0x0003
#include <net/ethernet.h>
#include <errno.h>   
#include <net/if.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
typedef struct _arp_hdr arp_hdr;
struct _arp_hdr {
  uint16_t htype;
  uint16_t ptype;
  uint8_t hlen;
  uint8_t plen;
  uint16_t opcode;
  uint8_t sender_mac[6];
  uint8_t sender_ip[4];
  uint8_t target_mac[6];
  uint8_t target_ip[4];
};
#define RARPOP_REPLY 4 
   
int main ()
{
  int sd, status; 
  char *ether_frame=(char*)malloc(1500);
  if ((sd = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_RARP))) < 0) 
  {
    perror ("socket() failed ");
    exit (EXIT_FAILURE);
  }
  else printf("%d\n",sd);
  struct sockaddr_ll device;
  int len=sizeof(device);
  memset (&device, 0, sizeof (device));
  char  interface[40];
	strcpy (interface,"ens33");
	if ((device.sll_ifindex = if_nametoindex (interface)) == 0)
	{
		perror ("if_nametoindex() failed to obtain interface index ");
		exit (EXIT_FAILURE);
	}
  device.sll_family = AF_PACKET;
  while(1)
  {
    status = recvfrom(sd, ether_frame,sizeof(ether_frame), 0,(struct sockaddr*)&device,&len);
    printf("hello\n");
    if(status<0)
    {
      printf("recverror\n");
      exit(0);
    }
    else printf("%d\n",status);
    struct iphdr ip=*(struct iphdr*)(ether_frame+sizeof(struct ethhdr));
    printf("%d\n",ip.protocol);
  }
  return 0;
}
