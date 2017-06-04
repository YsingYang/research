#include "routerForwardPacket.h"




void loopFunction(u_char *user, const struct pcap_pkthdr *packetHeader,const u_char *packetData);


int main(){
    /********************             init device       ******************************/
    char *device = "wlan0";
    char errbuf[1024];

    /********************             init pcap_t       ******************************/
    pcap_t *phandle;
    phandle = pcap_open_live(device, 65535, 1, 0, errbuf);

    /********************            broadcastFD    and  Buff   ******************************/
    broadcastFD = initBroadcastFD("wlan0");

    memset(FakeRTSFrame, 0, sizeof(FakeRTSFrame)); ///这里应该还是数组长度?
    memset(FakeBeaconFrame, 0, sizeof(FakeBeaconFrame));

    /********************             init UDPSocketFD           ******************************/
    sockFD = socket(AF_INET, SOCK_DGRAM, 0);
    clientAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.1.188", &clientAddr.sin_addr);///字符串中  填入目的服务器的地址
    clientAddr.sin_port = htons(6666);

    if(phandle==NULL){
        perror(errbuf);
        exit(EXIT_FAILURE);
    }

    pcap_loop(phandle, -1, loopFunction, (u_char *) "Ysing");

}




