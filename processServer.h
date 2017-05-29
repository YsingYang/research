
#ifndef PROCESSSERVER_H
#define PROCESSSERVER_H


#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "PNEpollManager.h"
#include "80211Packet.h"
//单例, 暂时没考虑线程安全

class processServer{

public:
    static processServer* Singleton();
    void serverProcess();
    void UDPUnpacking(int fd);
private:
    processServer();

    processServer(processServer &) = delete;
    processServer& operator=(processServer &) = delete;


    static processServer* singleServer;

    int TCPSocketFD;
    int UDPSocketFD;
    uint16_t UDPPort;
    uint16_t TCPPort;
    struct sockaddr_in TCPSocketAddr, UDPSocketAddr;
    PNEpollManager epollManager;
};


#endif // PROCESSSERVER_H
