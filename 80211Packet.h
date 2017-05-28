#ifndef _80211PACKET_H
#define _80211PACKET_H

#include "type.h"


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
    inline void setRadiotapHeader(u_char* data);
    inline void setFrame(u_char* data);

    void parse(int flag);

    void setParseProbeRequest(std::function<void()> func){
        parseProbeRequest = func;
    }
    void setParseBeacon(std::function<void()> func){
        parseBeacon = func;
    }

    void setParseRTS(std::function<void>() func){
        parseRTS = func;
    }

    void setParseCTS(std::function<void>() func){
        parseCTS = func;
    }

    void setParseQOSData(std::function<void>() func){
        parseQOSData = func;
    }

    void setParseData(std::function<void>() func){
        parseData = func;
    }


protected:

private:
    uint32_t radiotapHeaderLength;
    struct ieee80211_radiotap_header* radiotapHeader;
    struct ieee80211_hdr* frame;
    uint32_t frameLength;

    //用于给外部结构所需的解析函数
    std::function<void()> parseProbeRequest;
    std::function<void()> parseBeacon;
    std::function<void()> parseRTS;
    std::function<void()> parseCTS;
    std::function<void()> parseQOSData;
    std::function<void()> parseData;
};


inline void setRadiotapHeader(u_char* data){
    radiotapHeader = (struct ieee80211_radiotap_header* )(data);
}

inline void setFrame(u_char* data){
    frame = (ieee80211_hdr*) data;
}



#endif // 80211PACKET_H
