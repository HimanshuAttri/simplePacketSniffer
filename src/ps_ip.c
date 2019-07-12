#include <stdio.h>
#include "ps_ip.h"
#include "hash.h"

void ps_parse_tcp(const uint8_t *, const uint8_t);					// declaration

void ps_parse_ipv4(const uint8_t * packet)
{
	const ipv4_header_t *ip;
	int ip_version;
	int ip_size;

	ip = (ipv4_header_t*)(packet + ETHERNET_OFFSET);

	printf("       From: %s\n", inet_ntoa(ip->ip_src));
	printf("         To: %s\n", inet_ntoa(ip->ip_dst));

	ip_version = IP_V(ip);
	ip_size = IP_HL(ip)*4;

	printf("IP Version = %d\n", ip_version);

	// Find out the Protocol
	switch(ip->ip_p)
	{
		case 0x06:
			printf("   Protocol: TCP\n");
			ps_parse_tcp (packet, ip_size);							// If TCP, also parse it
			break;
		case 0x11:
			printf("   Protocol: UDP\n");
			break;
		case 0x01:
			printf("   Protocol: ICMP\n");
			break;
		default:
			printf("   Protocol: unknown\n");
			break;
	}
	ht_table_t *ht;
	l3l4_quin_t *quin;
	l3l4_quin_init(quin);
	//l3_extract_quin(quin, packet);
	    quin->dst_ip.un.v4.ip = *(uint32_t*)&packet[0];
    quin->src_ip.un.v4.ip = *(uint32_t*)&packet[6];
    quin->src_port = *(uint16_t*)&packet[34];
    quin->dst_port = *(uint16_t*)&packet[36];
    quin->proto =  *(uint8_t*)&packet[23];

	/*ht_table_t *ht;
	l3l4_quin_t *quin;
	l3l4_quin_init(quin);
	//extract quin
	l3_extract_quin(quin, packet);

	ht_init(ht);
	printf("%s\n","-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" );
//	ht_print(ht);*/



	
	return;
}



void ps_parse_ipv6 (const uint8_t * packet)
{
	const ipv6_header_t * ip;
	ip = (ipv6_header_t*)(packet + ETHERNET_OFFSET);

	char src_addr[16];

	printf("       From: %s\n", inet_ntop(AF_INET6, &(ip->ip_src), src_addr, 46));
	//printf("         To: %s\n", inet_ntop(ip->ip_dst));

}





void ps_parse_tcp(const uint8_t * packet, const uint8_t ip_size)
{
	tcp_header_t * tcp;
	tcp = (tcp_header_t*)(packet + ETHERNET_OFFSET + ip_size);

	printf("   Src port: %d\n", ntohs(tcp->src_port));
	printf("   Dst port: %d\n", ntohs(tcp->dst_port));
}
