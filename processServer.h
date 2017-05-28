
#ifndef PROCESSSERVER_H
#define PROCESSSERVER_H


#include <sys/socket.h>
#include <arpa/inet.h>
//单例
class processServer{

public:
    static processServer* Singleton();

private;
    processServer();

    processServer(processServer &) = delete;
    processServer& operator=(processServer &) = delete;

    static processServer* singleServer;
    int TCPSocketFD;
    int UDPSocketFD;
    uint16_t UDPPort;
    uint16_t TCPPort;
};

static processServer* processServer::singleServer = nullptr;


#endif // PROCESSSERVER_H
