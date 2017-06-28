#include "Device.h"
#include "ieee80211.h"
#include <string>
#include <assert.h>

bool device::recognizeMAC(const std::string &str){
    return !(str[1] == '2' || str[1] == '6' || str[1] == 'A' || str[1] == 'E');
}

std::string device::DecToHexStr(const std::string &addr){
    std::string HexStr;
    for(uint32_t i = 0; i < ETH_ALEN; ++i){
        for(int shift = 4; shift >= 0; shift -= 4){
            int num = addr[i] >> shift & 0xf;

            if(num >= 10)       HexStr += 'A' + num - 10;
            else    HexStr += std::to_string(num);

        }
        HexStr += ':';
    }
    HexStr.erase(HexStr.end() - 1);
    return HexStr;
}

device::device(std::string MAC , short cap, uint32_t pkSize, uint16_t seq, std::set<std::string> ssid) :
        capInfo(cap), MACKey(MAC), currentMAC(MAC), packetSize(pkSize), sequence(seq), SSIDList(ssid){
}

void device::update(std::shared_ptr<device> &dv){
    assert(capInfo == dv->getCapInfo() && packetSize == dv->getPacketSize());
    sequence = dv->getSeq();
    std::set<std::string> s_list = dv->getSSIDList();
    if(!s_list.empty()){
        for(auto &it : s_list){
            SSIDList.insert(it);
        }
    }
    currentMAC = dv->getCurrentMAC();
}
