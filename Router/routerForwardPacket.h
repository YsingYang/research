/******************************************

    由于单独作用在路由器上, 所以这里采用纯c编程.

******************************************/

#ifndef  ROUTEFORWARDPACKET_H
#define  ROUTEFORWARDPACKET_H

#include <pcap.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
//#include "ieee80211.h"
//#include "ieee80211_radiotap.h"


void loopFunction(u_char *user, const struct pcap_pkthdr *packetHeader,const u_char *packetData){
    uint32_t packetLength = packetHeader -> caplen;
    printf("%d \n", packetLength);
    //char packet[packetLength];
    //memcpy(packet, packetData, packetLength);
    //packet[packetLength] = '\0';
    int sockFD;
    struct sockaddr_in clientAddr;
    sockFD = socket(AF_INET, SOCK_DGRAM, 0);
    clientAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.1.188", &clientAddr.sin_addr);///字符串中  填入目的服务器的地址
    clientAddr.sin_port = htons(6666);
    sendto(sockFD,  packetData, packetLength, 0, (struct sockaddr*)(&clientAddr), sizeof(clientAddr));

    //udp转发至目的地


}



#endif
