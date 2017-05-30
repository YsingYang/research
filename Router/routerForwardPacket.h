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

/*void loopFunction(u_char *user, const struct pcap_pkthdr *packetHeader,const u_char *packetData){
    uint32_t packetLength = packetHeader -> caplen;
    printf("%d \n", packetLength);
    //char packet[packetLength];
    //memcpy(packet, packetData, packetLength);
    //packet[packetLength] = '\0';

    if(connect(sockFD, (sockaddr*)&clientAddr), sizeof(sockaddr) < 0){
        perror("connect error ");
        exit(1);
    }



    sendto(sockFD,  packetData, packetLength, 0, (struct sockaddr*)(&clientAddr), sizeof(clientAddr));

}*/



#endif
