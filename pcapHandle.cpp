#include "pcapHandle.h"

void LoopFunc(u_char* handleFileDescription, const struct pcap_pkthdr* packetHeader, const u_char* packetData){
    rt_header_t* rtHeader = reinterpret_cast<rt_header_t*>(const_cast<u_char*>(packetData)); ///会不会出bug?
    uint32_t rtLength = le16_to_cpu( rtHeader -> it_len); //这句需要?
    if(rtLength > packetHeader->caplen){
        printf("radiotap length exceeds package caplen");
        exit(0);
    }
    uint32_t fLength = packetHeader->caplen - rtLength;

    _80211Packet capturedPacket(rtLength, fLength);

    capturedPacket.setRadiotapHeader(rtHeader); //radiotap_header
    capturedPacket.setFrame((frame_t*)(rtHeader + rtLength));//frame body
    //开始解析
    capturedPacket.parse(0);
    printf("执行测试");

    ///定义需要调用的函数
}

pcapHandle::pcapHandle(std::string userName) : userName_(userName){
    char errorInfomation[1024];
    char *netcard = pcap_lookupdev(errorInfomation);
    if(netcard == nullptr){
        perror(errorInfomation);
        exit(1);
    }
    initFileDescription(const_cast<char *> (userName.c_str()), netcard, errorInfomation);
}

pcapHandle::pcapHandle(std::string userName, std::string netcard) : userName_(userName){
    char errorInfomation[1024];

    initFileDescription(const_cast<char *> (userName.c_str()), const_cast<char*>(netcard.c_str()), errorInfomation);
}


void pcapHandle::loop(int loopTimes){
    pcap_loop(handleFileDescription, loopTimes, LoopFunc, reinterpret_cast<u_char*>(const_cast<char*> (userName_.c_str())));
}

