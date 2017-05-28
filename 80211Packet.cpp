#include "80211Packet.h"




_80211Packet(uint32_t rtLen, uint32_t fLen) :  radiotapHeaderLength(rtLen), frameLength(fLen){

}

_80211Packet::parse(int flag){
    if((flag &  PROBE_REQUEST) && ieee80211_is_probe_req(frame -> frame_control)){
        parseProbeRequest();
        return;
    }

    if((flag  & BEACON) && ieee80211_is_beacon(frame->frame_control)){
        parseBeacon();
    }

    if((flag  & RTS ) && ieee80211_is_rts(frame -> frame_control)){
        parseRTS();
    }

    if((flag & CTS) && ieee80211_is_cts(frame->frame_control)){
        parseCTS();
    }

    if((flag & QOS_DATA) && ieee80211_is_data_qos(frame->frame_control)){
        parseQOSData();
    }

    if((flag & DATA ) && ieee80211_is_data(frame->frame_control)){
        parseData();
    }
}


