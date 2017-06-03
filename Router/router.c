#include "routerForwardPacket.h"
#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "ieee80211_radiotap.h"
#include "ieee80211.h"

#define le16_to_cpu __le16_to_cpu


void loopFunction(u_char *user, const struct pcap_pkthdr *packetHeader,const u_char *packetData);
int sockFD;
struct sockaddr_in clientAddr;


int main(){
    char *device = "wlp3s0b1";
    char errbuf[1024];

    pcap_t *phandle;

    phandle = pcap_open_live(device, 65535, 1, 0, errbuf);

    sockFD = socket(AF_INET, SOCK_DGRAM, 0);
    clientAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.1.188", &clientAddr.sin_addr);///字符串中  填入目的服务器的地址
    clientAddr.sin_port = htons(6666);

    if(phandle==NULL){
        perror(errbuf);
        exit(EXIT_FAILURE);
    }

    pcap_loop(phandle, -1, loopFunction, (u_char *) "Ysing");
}

void loopFunction(u_char *user, const struct pcap_pkthdr *packetHeader,const u_char *packetData){
    uint32_t packetLength = packetHeader -> caplen;
    for(int i = 0; i < packetLength ;++i){
        printf("%02x  ", packetData[i]);
    }

    if(sendto(sockFD,  packetData, packetLength, 0, (struct sockaddr*)(&clientAddr), sizeof(clientAddr)) < 0){
    	perror("send to error ");
    }
}



