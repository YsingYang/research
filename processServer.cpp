#include "processServer.h"


processServer* processServer::singleServer = nullptr;

processServer* processServer::Singleton(){
    if(singleServer == nullptr){
        singleServer = new processServer();
    }
    return singleServer;
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
    memset(static_cast<void*>(&TCPSocketAddr), 0, sizeof(sockaddr_in));
    memset(static_cast<void*>(&UDPSocketAddr), 0, sizeof(sockaddr_in));

    ///tcp端口5555, 没有指定ip地址
    TCPSocketAddr.sin_family = AF_INET;
    TCPSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    TCPSocketAddr.sin_port = htons(5555);
    TCPPort = 5555;

    ///udp端口为6666
    UDPSocketAddr.sin_family = AF_INET;
    UDPSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    UDPSocketAddr.sin_port = htons(6666);
    UDPPort = 6666;

    printf("accept from %s :%d \n", inet_ntoa(UDPSocketAddr.sin_addr), UDPSocketAddr.sin_port);


    if(bind(TCPSocketFD, (struct sockaddr *)&TCPSocketAddr, sizeof(TCPSocketAddr)) < 0){
        perror("TCP bind error");
        exit(1);
    }

    if(bind(UDPSocketFD, (struct sockaddr *)&UDPSocketAddr, sizeof(UDPSocketAddr)) < 0){
        perror("UDP bind error");
        exit(1);
    }

    ///加入进epoll监听事件
    printf("准备加入监听\n");
    epollManager.addEvent(TCPSocketFD, 0);
    epollManager.addEvent(UDPSocketFD, 0);
    listen(TCPSocketFD, 100);///tcp监听
}

void processServer::UDPUnpacking(int fd){
    char recvBuff[4096]; ///假设包不超过1024byte
    memset(recvBuff, 0, 4096);
    //不加入接受的while(循环)
    int packetSize;
    //用于记录接受的地址
    struct sockaddr_in routerAddr;
    socklen_t structLength = sizeof(routerAddr);
    memset((void*)&routerAddr, 0, structLength);

    if((packetSize = recvfrom(fd, recvBuff, 4096, 0,(sockaddr*)&routerAddr, &structLength)) < 0){
        //return;
    }
    //printf("执行测试 包大小为 %d \n", packetSize);

    rt_header_t* rtHeader = (rt_header_t*)(recvBuff); ///会不会出bug?
    for(int i = 0; i < packetSize; ++i){
        printf("%02X  ", recvBuff[i]);
    }
    printf("\n");
    /*uint16_t rtLength = le16_to_cpu( rtHeader -> it_len); //这句需要?
    printf("rtLength %d, packetSize %d\n",rtLength, packetSize);
    if(rtLength > packetSize){
        printf("radiotap length exceeds package caplen");
        return;
        exit(0);
    }
    uint32_t fLength = packetSize - rtLength;

    _80211Packet capturedPacket(rtLength, fLength);

    capturedPacket.setRadiotapHeader(rtHeader); //radiotap_header
    capturedPacket.setFrame((frame_t*)(rtHeader + rtLength));//frame body
    //开始解析
    capturedPacket.parse(0);*/
}

void processServer::serverProcess(){
    int recvReadyEvents;
    //printf("%d , and %d , %d \n",TCPSocketFD, UDPSocketFD, epollManager.epollFD);
    while(1){
        recvReadyEvents = epollManager.eventPoller();
        //printf("recvNum, %d\n", recvReadyEvents);
        for(int i = 0; i < recvReadyEvents; ++i){ ///暂时只写了UDP端口的监听
            if(epollManager.recvEvent[i].data.fd == UDPSocketFD)
                UDPUnpacking(UDPSocketFD);
        }
    }
}
