#include "PNEpollManager.h"
#include <stdlib.h>
#include <fcntl.h> //fcntl
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include<stdio.h>

PNEpollManager::PNEpollManager(){
    epollFD = epoll_create(EPOLL_CLOEXEC);//创建一个epollFD
    memset((void*)(recvEvent), 0, sizeof(recvEvent));
    //创建最大可创建数量
    if(epollFD <0){
        perror("epoll_create error  ");
    }
}

PNEpollManager::~PNEpollManager(){
}

int PNEpollManager::eventPoller(int waitMs){ //暂时只处理
    int num = epoll_wait(epollFD, recvEvent, maxEvents, waitMs);
    return num;
}

bool PNEpollManager::addEvent(const int fd, int op){ ///单单先考虑读与ET 模式,
    struct epoll_event temp;
    memset(&temp, 0, sizeof(temp));
    temp.data.fd = fd;
    temp.events = EPOLLIN;
        // 不判断小于0了
    setNoBlock(fd);
    if(epoll_ctl(epollFD, EPOLL_CTL_ADD, fd, &temp) < 0){
        perror("add event error");
    }
    return true;
}

/**
///取消了if语句 ,但不是这里的问题
**/
bool PNEpollManager::delEvent(const int fd, int op){
    close(fd);
    epoll_ctl(epollFD, EPOLL_CTL_DEL, fd, nullptr);
    return true;
}

