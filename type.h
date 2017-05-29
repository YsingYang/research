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

#define le16_to_cpu __le16_to_cpu

namespace hd80211{
    typedef struct ieee80211_radiotap_header rt_header_t;
    typedef struct ieee80211_hdr frame_t;
}

#endif // CAPTURETYPE_H
