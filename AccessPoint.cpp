#include "AccessPoint.h"


std::vector<std::vector<u_char>> accessPoint::APmap;

void accessPoint::addAccessPoint(){
    std::vector<u_char> addr1(6);
    memcpy(&addr1[0], "\xEC\x17\x2F\x2D\xB6\xB8", ETH_ALEN);
    APmap.emplace_back(addr1);
}
