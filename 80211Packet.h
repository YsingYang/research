#ifndef _80211PACKET_H
#define _80211PACKET_H

#include "type.h"
#include "ieee80211.h"

#define  PROBE_REQUEST 1
#define  BEACON 2
#define  RTS 4
#define  CTS 8
#define  QOS_DATA 16
#define  DATA 32


using namespace hd80211;

class _80211Packet{


public:
    _80211Packet(uint32_t rtLen, uint32_t fLen);

    //Shallow copy!!
    inline void setRadiotapHeader(rt_header_t* data);
    inline void setFrame(frame_t* data);

    void parse(int flag);

    static  void setParseProbeRequest(std::function<void()> func){
        parseProbeRequest = func;
    }
    static void setParseBeacon(std::function<void()> func){
        parseBeacon = func;
    }

    static void setParseRTS(std::function<void()> func){
        parseRTS = func;
    }

    static void setParseCTS(std::function<void()> func){
        parseCTS = func;
    }

    static void setParseQOSData(std::function<void()> func){
        parseQOSData = func;
    }

    static void setParseData(std::function<void()> func){
        parseData = func;
    }


protected:

private:
    uint32_t radiotapHeaderLength;
    struct ieee80211_radiotap_header* radiotapHeader;
    struct ieee80211_hdr* frame;
    uint32_t frameLength;

    //用于给外部结构所需的解析函数
    static std::function<void()> parseProbeRequest;
    static std::function<void()> parseBeacon;
    static std::function<void()> parseRTS;
    static std::function<void()> parseCTS;
    static std::function<void()> parseQOSData;
    static std::function<void()> parseData;
};


inline void _80211Packet::setRadiotapHeader(rt_header_t* data){
    radiotapHeader = data;
}

inline void _80211Packet::setFrame(frame_t* data){
    frame = data;
}



#endif // 80211PACKET_H
