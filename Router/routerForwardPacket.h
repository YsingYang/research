/******************************************

    由于单独作用在路由器上, 所以这里采用纯c编程.

******************************************/

#ifndef  ROUTEFORWARDPACKET_H
#define  ROUTEFORWARDPACKET_H

#define __SENDFAKERTS__
#define __SENDFAKEBEACON__

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
#include <assert.h>

#include "ieee80211.h"
#include "ieee80211_radiotap.h"

#define le16_to_cpu __le16_to_cpu

int recvRTSFrame = 0;
uint32_t FakeRTSLength;

int recvBeaconFrame = 0;
uint32_t  FakeBeaconLength;

u_char FakeRTSFrame[128];//一般RTS
u_char FakeBeaconFrame[1024];

char SSID_1[7] = "OpenWrt";
uint32_t ssidLength;

int sockFD;
int broadcastFD;
struct sockaddr_in clientAddr;

void sendRTSFrame();
void initBeaconPacket(const u_char* data, int packetLength, int radiotapLength);
void sendBeaconFrame();

void loopFunction(u_char *user, const struct pcap_pkthdr *packetHeader,const u_char *packetData){
    uint32_t packetLength = packetHeader -> caplen;
    struct ieee80211_radiotap_header* rtHeader = (struct ieee80211_radiotap_header*)packetData;
    int rtLength = le16_to_cpu(rtHeader->it_len);
    if(rtLength > packetLength){
        printf("exceeding packet size\n");
        return;
    }
    struct ieee80211_hdr *hdr = (struct ieee80211_hdr* )(packetData + rtLength);

    #ifdef __SENDFAKERTS__
    if(1 == ieee80211_is_rts(hdr->frame_control) && recvRTSFrame == 0){ //未拷贝
        memcpy(FakeRTSFrame, packetData, packetLength);
        FakeRTSLength = packetLength;
        recvRTSFrame = 1;
        struct ieee80211_rts* rtsBody = (struct ieee80211_rts*)(FakeRTSFrame + rtLength);
        memcpy(rtsBody->ra, "\xd0\xa6\x37\x0e\x12\x75", 6);
        memcpy(rtsBody->ta, "\xec\x17\x2f\x2d\xb6\xb9", 6);
    }
    #endif // __SENDFAKEBEACON__

    #ifdef __SENDFAKEBEACON__
    if(0 == recvBeaconFrame && 1 == ieee80211_is_beacon(hdr->frame_control)){ //Beacon帧未拷贝
        initBeaconPacket(packetData, packetLength, rtLength);
    }
    #endif // __SENDFAKEBEACON__

    if(sendto(sockFD,  packetData, packetLength, 0, (struct sockaddr*)(&clientAddr), sizeof(clientAddr)) < 0){
    	perror("send to error ");
    }
    if(recvRTSFrame){
        sendRTSFrame(sockFD, FakeRTSFrame, FakeRTSLength);
    }
    if(recvBeaconFrame){
        sendBeaconFrame(sockFD, FakeBeaconFrame, FakeBeaconLength);
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

inline void sendBeaconFrame(){
    if(write(broadcastFD, FakeBeaconFrame, FakeBeaconLength) < 0){
        perror("send 80211Beacon packet error");
    }
}

void initBeaconPacket(const u_char* data, int packetLength, int radiotapLength){
    memcpy(FakeBeaconFrame, data, packetLength); //先将帧memcpy一遍, 再进行修改
    FakeBeaconLength = packetLength;
    struct ieee80211_mgmt* mgmt = (struct ieee80211_mgmt*)(FakeBeaconFrame + radiotapLength);
    struct ieee80211_mgmt* originalMgmt = (struct ieee80211_mgmt*)(data + radiotapLength);

    memcpy(mgmt->sa, "\xec\x17\x2f\x2d\xb6\xb8", 6);//伪造Beacon的AP
    ssidLength =  strlen(SSID_1);//这样会不会有问题?
    printf("ssidLength = %d \n", ssidLength);

    struct ieee80211_ie *ie = (struct ieee80211_ie*)mgmt->u.beacon.variable;
    struct ieee80211_ie *originalIe = (struct ieee80211_ie*)originalMgmt->u.beacon.variable;

    uint32_t remainOffset = packetLength - radiotapLength; //两者共用一个remainoffset
    int modifyFlag = 1;
    while(remainOffset > FCS_LEN && modifyFlag){
        switch( ie->id ){
            case WLAN_EID_SSID:
                assert(ssidLength < IEEE80211_MAX_SSID_LEN);
                FakeBeaconLength +=  ssidLength - ie->len;
                ie->len = ssidLength;//需要修改长度
                memcpy((void*)(ie->data), SSID_1, ssidLength);
                modifyFlag = 0;//已修改
                break;
        }
        ie = (struct ieee80211_ie*)((u_char*)ie + ie->len + 2);
        originalIe = (struct ieee80211_ie*)((u_char*)originalIe + originalIe->len + 2);
        remainOffset -= originalIe->len - 2;
    }
    memcpy((void*)ie, (void*)originalIe, remainOffset);
    recvBeaconFrame = 1;//修改完成, 表示以伪造出一个Beacon帧
}
#endif
