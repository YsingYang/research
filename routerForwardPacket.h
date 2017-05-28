/******************************************

    由于单独作用在路由器上, 所以这里采用纯c编程.

******************************************/

#ifndef "ROUTEFORWARDPACKET_H"
#define "ROUTEFORWARDPACKET_H"

#include <pcap.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "ieee80211.h"
#include "ieee80211_radiotap.h"


void pcapHandler(u_char *user,const struct pcap_pkthdr *packetHeader,const u_char *packetData){
    uint32_t packetLength = packetHeader -> caplen;
    int sockFD;
    struct sockaddr_in clientAddr;
    socket(AF_INET, SOCK_DGRAM, 0);
    clientAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.1.188", &clientAddr.sin_addr) ///字符串中  填入目的服务器的地址
    clientAddr.sin_port = htons(6666);
    sendto(sockFD, (void*)(packetData), packetLength, 0, (sockaddr *)&clientAddr, sizof(clientAddr));

    //udp转发至目的地


}



#endif
