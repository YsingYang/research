#include "processServer.h"

static processServer* processServer::singleServer(){

}

processServer::processServer(){
    if( (TCPSocketFD = socket(AF_INET, SOCK_STREAM, 0))  < 0){
        perror("Get TCPFD error ");
        exit(1);
    }
    if((UDPSocketFD = socket(AF_INET, SOCK_DGRAM, 0))  < 0){
        perror("Get UDPFD error");
        exit(1);
    }
    memset(static_cast<void*>(&TCPSocetAddr), 0, sizeof(sockaddr_in));
    memset(static_cast<void*>(&UDPSocketAddr), 0, sizeof(sockaddr_in));

    ///tcp端口5555, 没有指定ip地址
    TCPSocetAddr.sin_family = AF_INET;
    TCPSocetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    TCPSocetAddr.sin_port = 5555;

    ///udp端口为6666
    UDPSocketAddr.sin_addr = AF_INET;
    UDPSocketAddr.sin_addr.s_addr = htol(INADDR_ANY);
    UDPSocetAddr.sin_port = 6666;

    bind(TCPSocketFD, (struct sockaddr *)&TCPSocetAddr, sizeof(TCPSocetAddr));

    if(bind(TCPSocketFD, )
}
