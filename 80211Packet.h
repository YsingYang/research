#ifndef _80211PACKET_H
#define _80211PACKET_H

#include "type.h"
#include "ieee80211.h"
#include "AccessPoint.h"
#include <memory>

#define  PROBE_REQUEST 1
#define  BEACON 2
#define  RTS 4
#define  CTS 8
#define  QOS_DATA 16
#define  DATA 32


using namespace Ysing;





class _80211Packet{
public:
    _80211Packet(uint32_t rtLen, uint32_t fLen);
    //deep copy!!
    inline void setRadiotapHeader(rt_header_t* data);
    void resetParseFunc(std::function<void(void* args)>);

    //需要重写的函数
    virtual void setFrameBody(u_char *) = 0;
    virtual void parse() = 0 ;
    virtual ~_80211Packet() = 0;


private:
    uint32_t radiotapHeaderLength;
    uint32_t frameLength;
    std::shared_ptr<ieee80211_radiotap_header> radiotapHeader;


protected:
    std::function<void(void* args)> parseFunc;
    //用于给外部结构所需的解析函数
};


inline void _80211Packet::setRadiotapHeader(rt_header_t* data){
    memcpy((void*)(radiotapHeader.get()),  (void*)data, sizeof(rt_header_t));
}

inline void _80211Packet::resetParseFunc(std::function<void(void* args)> func){
    parseFunc = func;
}


class _80211CTS : public _80211Packet{

public:
    _80211CTS(uint32_t rtLen, uint32_t fLen);
    ~_80211CTS() = default;

    // set radiotap函数不变

    inline void setFrameBody(u_char *data) override;
    void parse() override;

private:
    std::shared_ptr<cts_t> frameBody;
};

void _80211CTS::setFrameBody(u_char* data){
    memcpy((void*)(frameBody.get()), (void*)(data), sizeof(cts_t));
}

class _80211ProbeRequest : public _80211Packet{
    _80211ProbeRequest(uint32_t rtLen, uint32_t fLen);
    ~_80211ProbeRequest() = default;


    inline void setFrameBody(u_char* data) override;
    void parse() override;
private:
    std::shared_ptr<ieee80211_mgmt> frameBody;
    std::shared_ptr<ie_t> infoElement;
};

void _80211ProbeRequest::setFrameBody(u_char* data){
    memcpy((void*)(frameBody.get()), (void*)(data), sizeof(mgmt_t));
    infoElement = std::shared_ptr<ie_t>((ie_t*)(frameBody->u.probe_req.variable));
}

#endif // 80211PACKET_H
