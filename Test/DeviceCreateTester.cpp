#include "../processServer.h"
#include "../AccessPoint.h"

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
