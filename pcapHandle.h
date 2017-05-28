#ifndef PCAPHANDLE_H
#define PCAPHADLE_H


#include "type.h"
#include "80211Packet.h"


using namespace hd80211;

///改为单例模式会好一点
class pcapHandle{
public:
    pcapHandle(std::string userName = std::string("default"));
    pcapHandle(std::string userName, std::string netCard);
    void loop(int loopTimes = -1);

private:
    pcapHandle(pcapHandle &) = delete;
    pcapHandle& operator=(pcapHandle&) = delete;

    static void LoopFunc(u_char *, const struct pcap_pkthdr* packetHeader, const u_char* packetData);
    inline void initFileDescription(char* userName, char* netcard, char* errorInfomation);

private:
    pcap_t* handleFileDescription;
    struct ieee80211_hdr *hdr;
    struct ieee80211_radiotap_header *radiotapHeader;
    int radiotapLength;
    int packetLength;
    std::string userName_;
};


static void pcapHandle::LoopFunc(u_char* handleFileDescription, const struct pcap_pkthdr* packetHeader, const u_char* packetData);{
    struct rt_header_t rtHeader* = static_cast<rtheader_t*> packetData;
    uint32_t rtLength = ntohs( rtHeader -> it_len); //这句需要?
    if(rtLength > PacketHeader->caplen){
        printf("radiotap length exceeds package caplen");
        exit(0);
    }
    uint32_t fLength = PacketHeader->caplen - rtLength;

    _80211Packet capturedPacket(rtLength, fLength);

    capturedPacket.setRadiotapHeader(rtHeader); //radiotap_header
    capturedPacket.setFrame(rtHeader + rtLength);//frame body
    //开始解析
    capturedPacket.parse();


    ///定义需要调用的函数

}

inline void pcapHandle::initFileDescription(char* userName, char* netcard, char* errorInfomation){
    handleFileDescription = pcap_open_live(userName, 65535, 1, 0, netcard);
    if(handleFileDescription == nullptr){
        perror(errorInfomation);
        exit(1);
    }
}
#endif // PCAPHANDLE_H
