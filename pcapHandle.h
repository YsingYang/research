#ifndef PCAPHANDLE_H
#define PCAPHADLE_H


#include "type.h"
#include "80211Packet.h"


using namespace Ysing;

///改为单例模式会好一点
class pcapHandle{
public:
    pcapHandle(std::string userName = std::string("default"));
    pcapHandle(std::string userName, std::string netCard);
    void loop(int loopTimes = -1);

private:
    pcapHandle(pcapHandle &) = delete;
    pcapHandle& operator=(pcapHandle&) = delete;

    //void LoopFunc(u_char *, const struct pcap_pkthdr* packetHeader, const u_char* packetData);
    inline void initFileDescription(char* userName, char* netcard, char* errorInfomation);

private:
    pcap_t* handleFileDescription;
    struct ieee80211_hdr *hdr;
    struct ieee80211_radiotap_header *radiotapHeader;
    int radiotapLength;
    int packetLength;
    std::string userName_;
};


inline void pcapHandle::initFileDescription(char* userName, char* netcard, char* errorInfomation){
    handleFileDescription = pcap_open_live(netcard, 65535, 1, 0, errorInfomation);
    if(handleFileDescription == nullptr){
        perror(errorInfomation);
        exit(1);
    }
}
#endif // PCAPHANDLE_H
