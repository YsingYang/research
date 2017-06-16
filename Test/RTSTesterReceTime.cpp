#include "../processServer.h"
#include "../AccessPoint.h"

long timeStart;
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
    memcpy(&target[0], "\xEC\x17\x2F\x2D\xB6\xB9", ETH_ALEN);
    targetList.emplace_back(target);
    server->addTarget(targetList);
    timeStart = clock();
    server->serverProcess();

}

