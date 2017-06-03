#include "80211Packet.h"



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

_80211ProbeRequest::_80211ProbeRequest(uint32_t rtLen, uint32_t fLen):_80211Packet(rtLen, fLen), frameBody(nullptr){

}

//Probe Request解析函数
void _80211ProbeRequest::parse(){
    if(parseFunc != nullptr){
        parseFunc(this);
    }

    else{
        uint8_t sa[ETH_ALEN];
        memcpy(sa, frameBody->mgmt->sa, ETH_ALEN); //解析源地址
        std::vector<std::vector<u_char>> &ap = accessPoint::APmap;
        printf("I'm probe Request frame size %d , ra addr : %02x %02x %02x %02x %02x %02x \n",this->getFrameLength(), sa[0], sa[1], sa[2], sa[3], sa[4], sa[5]);
        for(uint32_t i = 0 ;i < ap.size(); ++i){
            if(memcmp(&ap[i][0], sa, ETH_ALEN) == 0){
                printf("cmp successfully \n");
            }
        }
    }
}

_80211Beacon::_80211Beacon(uint32_t rtLen, uint32_t fLen): _80211Packet(rtLen, fLen), frameBody(nullptr){

}

void _80211Beacon::parse(){
    if(parseFunc != nullptr){
        parseFunc(this);
    }
    else{
        uint8_t sa[ETH_ALEN];
        memcpy(sa, frameBody->mgmt->sa, ETH_ALEN); //解析源地址
        //std::vector<std::vector<u_char>> &ap = accessPoint::APmap;
        printf("I'm Beacon frame size %d , ra addr : %02x %02x %02x %02x %02x %02x \n",this->getFrameLength(), sa[0], sa[1], sa[2], sa[3], sa[4], sa[5]);
    }
}




