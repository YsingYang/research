#ifndef CAPTURETYPE_H
#define CAPTURETYPE_H

#include "ieee80211.h"
#include "ieee80211_radiotap.h"
#include <pcap.h>
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <functional>
#include <string.h>
#include <vector>
#define le16_to_cpu __le16_to_cpu

namespace Ysing{
    typedef struct ieee80211_radiotap_header rt_header_t;
    typedef struct ieee80211_hdr frame_t;
    typedef struct ieee80211_cts cts_t;
    typedef struct ieee80211_mgmt mgmt_t;
    typedef struct ieee80211_ie ie_t;
    typedef struct ieee80211_qos_hdr qos_t;


    class mgmtBody{
    public:
        mgmtBody(u_char* data) : mgmt(nullptr), ie(nullptr){
            mgmt = new mgmt_t();
            memcpy((void*)mgmt, data, sizeof(mgmt_t));
            ie = (ie_t*)mgmt->u.probe_req.variable;
        }

        ~mgmtBody(){
            delete mgmt;
        }

        mgmt_t* mgmt;
        ie_t* ie;
    };
}

#endif // CAPTURETYPE_H
