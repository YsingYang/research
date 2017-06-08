
#ifndef PROCESSSERVER_H
#define PROCESSSERVER_H


#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <memory>
#include "PNEpollManager.h"
#include "80211Packet.h"
#include "target.h"

//单例, 暂时没考虑线程安全

class targetControler;
class deviceSet;

class processServer{

public:
    static processServer* Singleton();
    void serverProcess();
    void UDPUnpacking(int fd);
    void addTarget(std::vector<std::vector<u_char>>& targetList);
    inline std::vector<std::vector<u_char>>& getTargetList();
    inline targetControler* getControl();

private:
    processServer();
    processServer(processServer &) = delete;
    processServer& operator=(processServer &) = delete;

    std::shared_ptr<_80211Packet> packetFactory(const int &, const int &, u_char*);

    static processServer* singleServer;

    int TCPSocketFD;
    int UDPSocketFD;
    uint16_t UDPPort;
    uint16_t TCPPort;
    struct sockaddr_in TCPSocketAddr, UDPSocketAddr;
    PNEpollManager epollManager;
    std::shared_ptr<_80211Packet> recvPacket; ///这种方法只适用于单线程
    std::unique_ptr<targetControler> controler;
    std::vector<std::vector<u_char>> targetList;
    std::unique_ptr<deviceSet> ownerDeviceSet;
};

targetControler* processServer::getControl(){
    return controler.get();
}


std::vector<std::vector<u_char>>& processServer::getTargetList(){
    return targetList;
}


#endif // PROCESSSERVER_H
