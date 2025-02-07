#include <pcap.h> 
#include <string.h> 
#include <stdlib.h> 
#include <signal.h>
#include "crc.h"
#include "ps_eth.h"
#include "ps_ip.h"

#define MAXBYTES2CAPTURE 2048 

pcap_dumper_t *pdumper;
pcap_t *descr = NULL; 

static eth_stats_t eth_stats;

//decl fun for Ctrl+C
void INThandler(int);

// Ctrl+C defination
void  INThandler(int sig) {
    int8_t c;

    signal(sig, SIG_IGN);
    printf("Exit?");
    c = getchar();
    if (c == 'y' || c == 'Y'){
        pcap_breakloop(descr);
        pcap_close(descr);
        ps_eth_stats_print(&eth_stats);
        exit(0);
    }
    else
        signal(SIGINT, INThandler);
}

/* processPacket(): Callback function called by pcap_loop() everytime a packet */
/* arrives to the network card. This function prints the captured raw data in  */
/* hexadecimal.                                                             */
void processPacket(u_char *dumpfile, const struct pcap_pkthdr* pkthdr, const u_char * packet){ 
    int i=0;
    static packet_counter = 0; 
    static bytes_counter = 0;
    eth_type_t eth;
    char * protocol;
    
    printf("Packet Count: %d\n", ++(packet_counter));
    bytes_counter += pkthdr->len;
    printf("Received Packet Size: %d\n", pkthdr->len); 
    printf("Cumulative: Packets %d: Bytes %d\n", packet_counter, bytes_counter);
    
    printf("Payload:\n"); 

    eth = ps_parse_eth(&eth_stats, packet);
    printf("ETH TYPE = 0x%04x \n", ntohs(eth));
    
    switch (ntohs(eth)) {
        case PS_ETH_TYPE_IPV4:
            ps_parse_ipv4(packet);
            break;

        case PS_ETH_TYPE_IPV6:
            printf("IPv6 packet :-");
            ps_parse_ipv6(packet);
            break;

        default:
            break;
    }
   
    /* save the packet on the dump file */
    pcap_dump(dumpfile, pkthdr, packet);


/*    for (i=0; i<pkthdr->len; i++){ 
        if ( isprint(packet[i]) ) /* If it is a printable character, print it */
/*            printf("%c ", packet[i]); 
        else 
            printf(". "); 
    
        if( (i%16 == 0 && i!=0) || i==pkthdr->len-1 ) 
            printf("\n"); 
    }
*/
    printf("================================== \n");

    return;
} 

/* main(): Main function. Opens network interface and calls pcap_loop() */
int main(int argc, char *argv[]) { 
    
    int i=0, count=0; 
    signal(SIGINT, INThandler);
 
    char errbuf[PCAP_ERRBUF_SIZE], *device=NULL; 
    memset(errbuf,0,PCAP_ERRBUF_SIZE); 

ps_eth_stats_init(&eth_stats);

    if( argc > 1){  /* If user supplied interface name, use it. */
        device = argv[1];
    }
    else {  /* Get the name of the first device suitable for capture */ 
        if ( (device = pcap_lookupdev(errbuf)) == NULL){
            fprintf(stderr, "ERROR: %s\n", errbuf);
            exit(1);
        }
    }

    printf("Opening device %s\n", device); 
 
    /* Open device in promiscuous mode */ 
    if ( (descr = pcap_open_live(device, MAXBYTES2CAPTURE, 1,  512, errbuf)) == NULL) {
        fprintf(stderr, "ERROR: %s\n", errbuf);
        exit(1);
    }
    pdumper = pcap_dump_open(descr, "test.pcap");//save to file

    /* Loop forever & call processPacket() for every received packet*/ 
    if ( pcap_loop(descr, 0, processPacket, (unsigned char *)pdumper) == -1){
       fprintf(stderr, "ERROR: %s\n", pcap_geterr(descr) );
       exit(1);
    }
    return 0; 
} 
