#include "pcapHandle.h"
#include "processServer.h"
#include "AccessPoint.h"


//#define __CALLBACK__

#ifdef __CALLBACK__
void callback(void* args){
    _80211Beacon* event = reinterpret_cast<_80211Beacon*>(args);
    uint8_t sa[ETH_ALEN];
    memcpy(sa, event->frameBody->mgmt->sa, ETH_ALEN); //解析源地址
    //std::vector<std::vector<u_char>> &ap = accessPoint::APmap;
    printf("I'm Beacon frame size %d , ra addr : %02x %02x %02x %02x %02x %02x \n",event->getFrameLength(), sa[0], sa[1], sa[2], sa[3], sa[4], sa[5]);
}
#endif // __CALLBACK__

int main(){
    #ifdef __CALLBACK__
    _80211Beacon::resetParseFunc(callback);
    #endif // __CALLBACK__
    accessPoint::addAccessPoint();
    processServer *server = processServer::Singleton();
    std::vector<std::vector<u_char>> targetList;
    std::vector<u_char> target(6);
    memcpy(&target[0], "\xd0\xa6\x37\x0e\x12\x75", ETH_ALEN);
    targetList.emplace_back(target);
    //targetList.emplace_back(reinterpret_cast<std::vector<u_char>>(std::vector<char>({'\xEC', '\x17', '\x2F', '\x2D', '\xB6', '\xB8'})));
    //targetList.emplace_back(reinterpret_cast<std::vector<u_char>>(std::vector<char>({'\xd0', '\xa6', '\x37', '\x0e', '\x12', '\x75'})));
    server->addTarget(targetList);
    server->serverProcess();

}
