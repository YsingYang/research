//#define __DEBUG__

#include "80211Packet.h"
#include "processServer.h"
#include "PNTimeStamp.h"
#include "Device.h"
#include <assert.h>
#include <iostream>



std::function<void(void* args)> _80211Beacon::parseFunc = nullptr;

void _80211Beacon::resetParseFunc(std::function<void(void* args)> func){
    parseFunc = func;
}

std::function<void(void* args)> _80211CTS::parseFunc = nullptr;

void _80211CTS::resetParseFunc(std::function<void(void* args)>func){
    parseFunc = func;
}

std::function<void(void* args)> _80211ProbeRequest::parseFunc = nullptr;

void _80211ProbeRequest::resetParseFunc(std::function<void(void* args)> func){
    parseFunc = func;
}

std::function<void(void* args)> _80211QOSData::parseFunc = nullptr;

void _80211QOSData::resetParseFunc(std::function<void(void* args)> func){
    parseFunc = func;
}

//Packet构造函数
_80211Packet::_80211Packet(uint32_t rtLen, uint32_t fLen,uint16_t t, processServer* server_) :
    radiotapHeaderLength(rtLen), frameLength(fLen), type(t), radiotapHeader(new ieee80211_radiotap_header), server(server_){
}

 _80211Packet::~_80211Packet(){}


std::shared_ptr<device> _80211Packet::sptrParse(){
    printf("Do nothing \n");
    return std::shared_ptr<device>();
}


//CTS帧构造函数
_80211CTS::_80211CTS(uint32_t rtLen, uint32_t fLen, uint16_t t, processServer* server_):_80211Packet(rtLen, fLen, t, server_), frameBody(new cts_t){

}

void _80211CTS::setFrameBody(u_char* data, uint32_t dataLength){
    memcpy((void*)(frameBody.get()), (void*)(data), sizeof(cts_t));
}

//CTS解析函数
void _80211CTS::parse(){
    if(parseFunc != nullptr){ //修改了解析函数
        #ifdef __DEBUG__
        printf("Doing callback in CTS\n");
        #endif // __DEBUG__
        parseFunc(this);
    }
    else{
        uint8_t ra[ETH_ALEN];
        memcpy(ra, frameBody->ra, ETH_ALEN);
        std::vector<std::vector<u_char>>  &ap = accessPoint::APmap;
       // printf("CTS frame size , ra addr : %02x %02x %02x %02x %02x %02x \n", ra[0], ra[1], ra[2], ra[3], ra[4], ra[5]);

        for(uint32_t i = 0; i < ap.size(); ++i){
            if(memcmp(&ap[i][0], ra, ETH_ALEN) == 0){
                printf("cmp successfully  and time %s \n", PNTimestamp(PNTimestamp::now()).toFormattedString().c_str());
            }
        }
    }
}

_80211ProbeRequest::_80211ProbeRequest(uint32_t rtLen, uint32_t fLen, uint16_t t, processServer* server_):_80211Packet(rtLen, fLen, t, server_), frameBody(nullptr){

}

void _80211ProbeRequest::setFrameBody(u_char* data, uint32_t dataLength){
    frameBody = std::shared_ptr<mgmtBody>(new mgmtBody(data, dataLength));
}


//Probe Request解析函数
void _80211ProbeRequest::parse(){
    if(parseFunc != nullptr){
        #ifdef __DEBUG__
        printf("Doing callback in Probe\n");
        #endif // __DEBUG__
        parseFunc(this);
    }

    else{
        uint8_t sa[ETH_ALEN];
        memcpy(sa, frameBody->mgmt->sa, ETH_ALEN); //解析源地址
        processServer* ownerServer = getPacketServer();

        std::vector<std::vector<u_char>> &targetList = ownerServer->getTargetList();
        printf("I'm probe Request frame size %d , ra addr : %02x %02x %02x %02x %02x %02x \n",this->getFrameLength(), sa[0], sa[1], sa[2], sa[3], sa[4], sa[5]);
        for(uint32_t i = 0; i < targetList.size(); ++i){
            printf("compare probe request %02x %02x %02x %02x %02x %02x \n ", targetList[i][0], targetList[i][1], targetList[i][2], targetList[i][3], targetList[i][4], targetList[i][5]);
            if(memcmp(sa, &targetList[i][0], ETH_ALEN) == 0){
                printf("compare successfully\n");
            }
        }
    }
}


std::shared_ptr<device> _80211ProbeRequest::sptrParse(){
    struct ieee80211_ie* ie = static_cast<struct ieee80211_ie*> (frameBody->ie);
    uint32_t ssidLength = 0;
    int remain = getFrameLength() - 24;
    std::string MACAddress = device::DecToHexStr(std::string((char*)(&(frameBody->mgmt->sa[0])), 6));
    std::set<std::string> ssidList;
    short capInfo = -1;
    uint16_t seq = le16_to_cpu(frameBody->mgmt->seq_ctrl); //获取seq;
    while(remain > FCS_LEN){
        switch(ie->id){
            case WLAN_EID_SSID:
                ssidLength = static_cast<int>(ie->len);
                if(ssidLength != 0){
                    ssidList.insert(std::string((char*)(ie->data), ssidLength));
                }
                break;
            case WLAN_EID_HT_CAPABILITY :
                struct ieee80211_ht_cap* cap = (struct ieee80211_ht_cap*)(ie->data);
                capInfo = le16_to_cpu(cap->cap_info);//获取cap信息
                break;
        }
        remain -= ie->len + 2;
        ie = (struct ieee80211_ie*)((u_char*)(ie) + ie->len + 2);//下一个循环
    }
    uint32_t pkSize = getFrameLength() - ssidLength;
    assert(pkSize > 0);
    return std::shared_ptr<device>(new device(MACAddress, capInfo, pkSize, seq, ssidList));
}

_80211Beacon::_80211Beacon(uint32_t rtLen, uint32_t fLen, uint16_t t, processServer* server_): _80211Packet(rtLen, fLen, t, server_), frameBody(nullptr){

}

void _80211Beacon::setFrameBody(u_char* data, uint32_t dataLength){
    frameBody = std::shared_ptr<mgmtBody>(new mgmtBody(data, dataLength));
}

void _80211Beacon::parse(){
    if(parseFunc != nullptr){
        #ifdef __DEBUG__
        printf("Doing callback in Beacon\n");
        #endif
        parseFunc(this);
    }
    else{
        uint8_t sa[ETH_ALEN];
        memcpy(sa, frameBody->mgmt->sa, ETH_ALEN); //解析源地址
        //std::vector<std::vector<u_char>> &ap = accessPoint::APmap;
        printf("I'm Beacon frame size %d , ra addr : %02x %02x %02x %02x %02x %02x \n",this->getFrameLength(), sa[0], sa[1], sa[2], sa[3], sa[4], sa[5]);
    }
}


_80211QOSData::_80211QOSData(uint32_t rtLen, uint32_t fLen, uint16_t t, processServer* server_):_80211Packet(rtLen, fLen, t, server_), frameBody(nullptr){

}

void _80211QOSData::setFrameBody(u_char* data, uint32_t dataLength){
    frameBody = std::shared_ptr<qos_t>(new qos_t);
    memcpy((void*)(frameBody.get()), data, sizeof(qos_t));
}

void _80211QOSData::parse(){
    if(parseFunc != nullptr){
        #ifdef __DEBUG__
        printf("Doing callback in QoS\n");
        #endif // __DEBUG__
        parseFunc(this);
    }
    else{
            uint8_t BSS[ETH_ALEN];
            uint8_t Source[ETH_ALEN];
            uint8_t Destination[ETH_ALEN];
            memcpy(BSS, frameBody->addr1, ETH_ALEN);
            memcpy(Source, frameBody->addr2, ETH_ALEN);
            memcpy(Destination, frameBody->addr3, ETH_ALEN);
            printf("I'm QoS Data, BSS Addr : %02x %02x %02x %02x %02x %02x \n ", BSS[0], BSS[1], Source[0], Source[1], Destination[0], Destination[1]);
    }
}



