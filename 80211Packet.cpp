#include "80211Packet.h"


/*std::function<void()> _80211Packet::parseProbeRequest = nullptr;
std::function<void()> _80211Packet::parseBeacon = nullptr;
std::function<void()> _80211Packet::parseRTS = nullptr;
std::function<void()> _80211Packet::parseCTS = nullptr;
std::function<void()> _80211Packet::parseQOSData = nullptr;
std::function<void()> _80211Packet::parseData =nullptr ;
*/

//Packet构造函数
_80211Packet::_80211Packet(uint32_t rtLen, uint32_t fLen) :
    radiotapHeaderLength(rtLen), frameLength(fLen), radiotapHeader(new ieee80211_radiotap_header), parseFunc(nullptr){
}

 _80211Packet::~_80211Packet(){}




//CTS帧构造函数
_80211CTS::_80211CTS(uint32_t rtLen, uint32_t fLen):_80211Packet(rtLen, fLen), frameBody(new cts_t){

}

//CTS解析函数
void _80211CTS::parse(){
    if(parseFunc != nullptr){ //修改了解析函数
        parseFunc(this);
    }
    else{
        uint8_t ra[ETH_ALEN];
        memcpy(ra, frameBody->ra, ETH_ALEN);
        std::vector<std::vector<u_char>>  &ap = accessPoint::APmap;
        printf("frame size , ra addr : %02x %02x %02x %02x %02x %02x \n", ra[0], ra[1], ra[2], ra[3], ra[4], ra[5]);
        for(uint32_t i = 0; i < ap.size(); ++i){
            if(memcmp(&ap[i][0], ra, ETH_ALEN) == 0){
                printf("cmp successfully \n");
            }
        }
    }
}





