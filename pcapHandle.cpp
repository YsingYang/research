#include "pcapHandle.h"


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
