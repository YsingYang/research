#ifndef CAPTURETYPE_H
#define CAPTURETYPE_H

#include "ieee80211.h"
#include "ieee80211_radiotap.h"
#include <pcap.h>
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>



namespace hd80211{
    typedef ieee80211_radiotap_header rt_header_t;
    typedef ieee80211_hdr frame_t;
}

#endif // CAPTURETYPE_H
