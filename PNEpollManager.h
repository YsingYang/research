#ifndef PNEPOLLMANAGER_H
#define PNEPOLLMANAGER_H

#include <sys/epoll.h>
#include <string.h>
#include <stdio.h>
#include <list>
#include <fcntl.h>

class processServer;

const int maxEvents = 5000; //最大事件数
/***
    需要添加一个管理fd的集合, 以便析构
**/
class PNEpollManager{
    friend processServer;///先暂定设服务器类为该类的友元

public:
    PNEpollManager();
    ~PNEpollManager();

    bool addEvent(const int fd, int op);
    bool delEvent(const int fd, int op);
    bool  setEvent(const int fd, int op);

    int eventPoller(int waitMs = -1);
    inline int getMaxEvents() const;


private:
    PNEpollManager(const PNEpollManager&) = delete;
    PNEpollManager& operator = (const PNEpollManager &) = delete;

    inline bool setNoBlock(const int &) ; //如果为ET模式需要将fd设成非阻塞

private:
    int epollFD;
    struct epoll_event recvEvent[maxEvents];
};


inline int PNEpollManager::getMaxEvents() const{
    return maxEvents;
}

inline bool PNEpollManager::setNoBlock(const int &fd){
     if(fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0)|O_NONBLOCK) == -1){
        return false;
    }
    return true;
}



#endif // PNEPOLLMANAGER_H
