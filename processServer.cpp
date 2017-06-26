#define __DEBUG__
#define __SPTR_FUNCTOR__

#include "processServer.h"
#include "target.h"
#include "DeviceSet.h"
#include "Device.h"
#include <iostream>

processServer* processServer::singleServer = nullptr;

processServer* processServer::Singleton(){
    if(singleServer == nullptr){
        singleServer = new processServer();
    }
    return singleServer;
}

processServer::processServer() : recvPacket(nullptr), controler(nullptr), ownerDeviceSet(new deviceSet()){
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
    controler = std::unique_ptr<targetControler> (targetControler::Singleton());
    //这里写暂时写丑一点, 主要是用于初始化target列表, 不过没想到有什么好方法, 或者在哪里有list列
}

void processServer::UDPUnpacking(int fd){
    u_char recvBuff[4096]; ///假设包不超过1024byte
    memset(recvBuff, 0, 4096);
    //不加入接受的while(循环)
    int packetSize;
    //用于记录接受的地址
    struct sockaddr_in routerAddr;
    socklen_t structLength = sizeof(routerAddr);
    memset((void*)&routerAddr, 0, structLength);

    if((packetSize = recvfrom(fd, recvBuff, 4096, 0,(sockaddr*)&routerAddr, &structLength)) < 0){
        printf("recv error \n");
        return;
        //return;
    }
    rt_header_t* rtHeader = (rt_header_t*)(recvBuff); ///会不会出bug?
    int rtLength = le16_to_cpu( rtHeader -> it_len); //这句需要?
    if(rtLength > packetSize){
        return;
        exit(0);
    }
    int fLength = packetSize - rtLength;

    recvPacket = packetFactory(rtLength, fLength, recvBuff);

     #ifdef  __DEBUG__
    if(recvPacket != nullptr){
        recvPacket->setRadiotapHeader(rtHeader);
        recvPacket->setFrameBody(recvBuff + rtLength, fLength);
        if(ieee80211_is_cts(recvPacket->getType())){ //解析CTS
            recvPacket->parse();
        }

        //假设是probe Request, 收集数据集
        #ifdef  __SPTR_FUNCTOR__
        if(ieee80211_is_probe_req(recvPacket->getType())){
            std::shared_ptr<device> newDevice = recvPacket->sptrParse(); //获取到该设备的信息, 添加到
            ///1. 找deviceSet中是否有相应的key

        }
        #endif // __SPTR_FUNCTOR__
    }

    #endif // __DEBUG__
}

//Packet工厂
std::shared_ptr<_80211Packet> processServer::packetFactory(const int &rtLength, const int &fLength, u_char* recvBuff){
    frame_t* frameBody = (frame_t*)(recvBuff + rtLength);
    std::shared_ptr<_80211Packet> product = nullptr;
    if(ieee80211_is_cts(frameBody->frame_control) == 1){ ///这里放在用工厂模式会好点
        product =  std::shared_ptr<_80211CTS>(new _80211CTS(rtLength, fLength, frameBody->frame_control, this));
    }
    else if(ieee80211_is_probe_req(frameBody->frame_control) == 1){
        product = std::shared_ptr<_80211ProbeRequest>(new _80211ProbeRequest(rtLength, fLength, frameBody->frame_control, this));
    }
    else if(ieee80211_is_beacon(frameBody->frame_control) == 1){
        product = std::shared_ptr<_80211Beacon>(new _80211Beacon(rtLength, fLength, frameBody->frame_control, this));
    }
    else if(ieee80211_is_data_qos(frameBody->frame_control) == 1){
        product  = std::shared_ptr<_80211QOSData>(new _80211QOSData(rtLength, fLength, frameBody->frame_control, this));
    }
    return product;
}


//服务器类的接受, 暂时只处理UDP包
void processServer::serverProcess(){
    int recvReadyEvents;
    while(1){
        recvReadyEvents = epollManager.eventPoller();
        for(int i = 0; i < recvReadyEvents; ++i){ ///暂时只写了UDP端口的监听
            if(epollManager.recvEvent[i].data.fd == UDPSocketFD)
                UDPUnpacking(UDPSocketFD);
        }
    }
}

void processServer::addTarget(std::vector<std::vector<u_char>>& target){
    for(uint32_t i = 0; i < target.size(); ++i){
        targetList.emplace_back(target[i]);
    }
}
