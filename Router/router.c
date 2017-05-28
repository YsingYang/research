#include "routerForwardPacket.h"
#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>

void loopFunction(u_char *user, const struct pcap_pkthdr *packetHeader,const u_char *packetData);

int main(){
    char *device = "wlan0";
    char errbuf[1024];

    pcap_t *phandle;

    phandle = pcap_open_live(device, 65535, 1, 0, errbuf);

    if(phandle==NULL){
        perror(errbuf);
        exit(EXIT_FAILURE);
    }

    pcap_loop(phandle, -1, loopFunction, (u_char *) "Ysing");
}
