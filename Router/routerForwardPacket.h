/******************************************

    由于单独作用在路由器上, 所以这里采用纯c编程.

******************************************/

#ifndef  ROUTEFORWARDPACKET_H
#define  ROUTEFORWARDPACKET_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pcap.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/if_ether.h>
#include <netpacket/packet.h>
#include<net/if.h>

#include "ieee80211.h"
#include "ieee80211_radiotap.h"

#define le16_to_cpu __le16_to_cpu

int recvRTSFrame = 0;
uint32_t FakeRTSLength;

u_char FakeRTSFrame[128];//一般RTS
u_char FakeBeaconFrame[1024];

int sockFD;
int broadcastFD;
struct sockaddr_in clientAddr;

void sendRTSFrame();

void loopFunction(u_char *user, const struct pcap_pkthdr *packetHeader,const u_char *packetData){
    uint32_t packetLength = packetHeader -> caplen;
    struct ieee80211_radiotap_header* rtHeader = (struct ieee80211_radiotap_header*)packetData;
    int rtLength = le16_to_cpu(rtHeader->it_len);
    if(rtLength > packetLength){
        printf("exceeding packet size\n");
        return;
    }
    struct ieee80211_hdr *hdr = (struct ieee80211_hdr* )(packetData + rtLength);
    if(1 == ieee80211_is_rts(hdr->frame_control) && recvRTSFrame == 0){ //未拷贝
        memcpy(FakeRTSFrame, packetData, packetLength);
        FakeRTSLength = packetLength;
        recvRTSFrame = 1;
        struct ieee80211_rts* rtsBody = (struct ieee80211_rts*)(FakeRTSFrame + rtLength);
        memcpy(rtsBody->ra, "\xd0\xa6\x37\x0e\x12\x75", 6);
        memcpy(rtsBody->ta, "\xec\x17\x2f\x2d\xb6\xb9", 6);
    }

    if(sendto(sockFD,  packetData, packetLength, 0, (struct sockaddr*)(&clientAddr), sizeof(clientAddr)) < 0){
    	perror("send to error ");
    }
    if(recvRTSFrame){
        sendRTSFrame(sockFD, FakeRTSFrame, FakeRTSLength);
    }
}


int initBroadcastFD(char* interface){
    int sockFD = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sockFD < 0){
        perror("Create raw socket error");
        return -1;
    }
    struct ifreq interfaceRequest;
    memset((void*)&interfaceRequest, 0, sizeof(struct ifreq));
    strncpy(interfaceRequest.ifr_name, interface, sizeof(interfaceRequest.ifr_name) - 1);
    if(ioctl(sockFD, SIOGIFINDEX, &interfaceRequest) < 0){
        perror("SIOGIFINDEX error");
        return -1;
    }
    /********************bind raw socket to interface************************/
    struct sockaddr_ll sockAddr;
    memset((void*)&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sll_family = AF_PACKET;
    sockAddr.sll_ifindex = interfaceRequest.ifr_ifindex;
    sockAddr.sll_protocol = htons(ETH_P_ALL);
    if(bind(sockFD, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0){
        perror("Raw socket binding error");
        return -1;
    }
    /********************Open promise*************************************/
    struct packet_mreq promiseSet;
    memset((void*)&promiseSet, 0, sizeof(promiseSet));
    promiseSet.mr_ifindex = sockAddr.sll_ifindex;
    promiseSet.mr_type = PACKET_MR_PROMISC;
    if(setsockopt(sockFD, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &promiseSet, sizeof(promiseSet)) < 0){
        perror("setsocketopt (promise ) failed");
        return -1;
    }
    return sockFD;
}


inline void sendRTSFrame(){
    if(write(broadcastFD, FakeRTSFrame, FakeRTSLength) < 0){
        perror("send 80211 packet error");
    }
}
#endif
