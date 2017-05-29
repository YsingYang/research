#include "pcapHandle.h"

int main(){
    //char buff[1024];
    //printf("%s", pcap_lookupdev(buff));
    pcapHandle pcap(std::string("Ysing"), "wlp3s0b1");
    pcap.loop(100);
}
