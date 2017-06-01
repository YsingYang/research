#include "80211Packet.h"


/*std::function<void()> _80211Packet::parseProbeRequest = nullptr;
std::function<void()> _80211Packet::parseBeacon = nullptr;
std::function<void()> _80211Packet::parseRTS = nullptr;
std::function<void()> _80211Packet::parseCTS = nullptr;
std::function<void()> _80211Packet::parseQOSData = nullptr;
std::function<void()> _80211Packet::parseData =nullptr ;
*/

_80211Packet::_80211Packet(uint32_t rtLen, uint32_t fLen) :
    radiotapHeaderLength(rtLen), frameLength(fLen), radiotapHeader(new ieee80211_radiotap_header), frame(new ieee80211_hdr){
}

 _80211Packet::~_80211Packet(){

}

void _80211Packet::parse(int flag){
    /*if((flag &  PROBE_REQUEST) && ieee80211_is_probe_req(frame -> frame_control)){
        parseProbeRequest();
        return;
    }

    if((flag  & BEACON) && ieee80211_is_beacon(frame->frame_control)){
        parseBeacon();
    }

    if((flag  & RTS ) && ieee80211_is_rts(frame -> frame_control)){
        parseRTS();
    }

    if((flag & QOS_DATA) && ieee80211_is_data_qos(frame->frame_control)){
        parseQOSData();
    }

    if((flag & DATA ) && ieee80211_is_data(frame->frame_control)){
        parseData();
    }*/

    if(ieee80211_is_cts(frame->frame_control) == 1){
        struct ieee80211_cts* CTSFrame = (struct ieee80211_cts*)(&(*frame));
        uint8_t ra[ETH_ALEN];
        memcpy(ra, CTSFrame->ra, ETH_ALEN);
        std::vector<std::vector<u_char>>  &ap = accessPoint::APmap;
        printf("frame ra addr : %02x %02x %02x %02x %02x %02x \n",ra[0], ra[1], ra[2], ra[3], ra[4], ra[5]);
        for(uint32_t i = 0; i < ap.size(); ++i){
            if(memcmp(&ap[i][0], ra, ETH_ALEN) == 0){
                printf("cmp successfully \n");
            }
            //printf("%02x %02x %02x %02x %02x %02x \n",ap[i][0], ap[i][1], ap[i][2], ap[i][3], ap[i][4], ap[i][5]);
        }
    }

}

void implementation(u_char* data){

}
