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
    void parseRTSFrame();
    void parse(int flag);

protected:

private:
    uint32_t radiotapHeaderLength;
    struct ieee80211_radiotap_header* radiotapHeader;
    struct ieee80211_hdr* frame;
    uint32_t frameLength;
};


inline void setRadiotapHeader(u_char* data){
    radiotapHeader = (struct ieee80211_radiotap_header* )(data);
}

inline void setFrame(u_char* data){
    frame = (ieee80211_hdr*) data;
}



void parseRTSFrame(){

}


#endif // 80211PACKET_H
