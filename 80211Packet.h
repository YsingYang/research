#ifndef _80211PACKET_H
#define _80211PACKET_H

#include "type.h"
#include "ieee80211.h"
#include "AccessPoint.h"
#include <memory>

#define  PROBE_REQUEST 1
#define  BEACON 2
#define  RTS 4
#define  CTS 8
#define  QOS_DATA 16
#define  DATA 32


using namespace Ysing;



class processServer;
//所有802.11包的基类
class _80211Packet{
public:
    _80211Packet(uint32_t rtLen, uint32_t fLen, processServer* );
    //deep copy!!
    inline processServer* getPacketServer() const;
    inline void setRadiotapHeader(rt_header_t* data);
    inline uint32_t getFrameLength () const;
    inline uint32_t getRadiotapHeaderLength () const;
    inline std::weak_ptr<rt_header_t> getRadiotapBody() const;

    //需要重写的函数
    virtual void setFrameBody(u_char *) = 0;
    virtual void parse() = 0 ;
    virtual ~_80211Packet() = 0;

private:
    _80211Packet(const _80211Packet&) = delete;
    _80211Packet& operator=(const _80211Packet&) = delete;

    uint32_t radiotapHeaderLength;
    uint32_t frameLength;
    std::shared_ptr<ieee80211_radiotap_header> radiotapHeader;
    processServer* server;//指向所属的服务器, 不用管理析构
protected:
    //用于给外部结构所需的解析函数
};

processServer* _80211Packet::getPacketServer() const{
    return server;
}

std::weak_ptr<rt_header_t> _80211Packet::getRadiotapBody() const{
    return std::weak_ptr<rt_header_t>(radiotapHeader);
}


inline uint32_t _80211Packet::getFrameLength () const{
    return frameLength;
}

inline uint32_t _80211Packet::getRadiotapHeaderLength () const{
    return radiotapHeaderLength;
}

inline void _80211Packet::setRadiotapHeader(rt_header_t* data){
    memcpy((void*)(radiotapHeader.get()),  (void*)data, sizeof(rt_header_t));
}

//CTS类
class _80211CTS : public _80211Packet{
public:
    _80211CTS(uint32_t rtLen, uint32_t fLen, processServer* );
    ~_80211CTS() = default;

    inline std::weak_ptr<cts_t> getFrameBody() const;
    // set radiotap函数不变
    static void resetParseFunc(std::function<void(void* args)>);
    void setFrameBody(u_char *data) override;
    void parse() override;

private:
    std::shared_ptr<cts_t> frameBody;
    static std::function<void(void* args)> parseFunc;
};

std::weak_ptr<cts_t> _80211CTS::getFrameBody() const{
    return std::weak_ptr<cts_t>(frameBody);
}


//ProbeRequest类
class _80211ProbeRequest : public _80211Packet{
public:
    _80211ProbeRequest(uint32_t rtLen, uint32_t fLen, processServer* );
    ~_80211ProbeRequest() = default;

    static void resetParseFunc(std::function<void(void* args)>);

    void setFrameBody(u_char* data) override;
    void parse() override;
private:
    std::shared_ptr<mgmtBody> frameBody;
    static std::function<void(void* args)> parseFunc;
};

class _80211Beacon : public _80211Packet{
public:
    _80211Beacon(uint32_t rtLen, uint32_t fLen, processServer* );
    ~_80211Beacon() = default;

    inline std::weak_ptr<mgmtBody> getFrameBody() const;

    static void resetParseFunc(std::function<void(void* args)>);

    void setFrameBody(u_char* data) override;
    void parse() override;


private:
    std::shared_ptr<mgmtBody> frameBody;
    static std::function<void(void* args)> parseFunc;
};

std::weak_ptr<mgmtBody> _80211Beacon::getFrameBody() const{
    return std::weak_ptr<mgmtBody> (frameBody);
}

//对于QOSdata有3个地址, 第一个为BSS地址, 不管是设备到AP 还是AP ->设备, 地址都为AP, 之后的地址就是, 我们可以根据源地址与目的地址哪个与QOSBSSdata不同,
//就能得到连接的设备地址了
class _80211QOSData : public _80211Packet{
public:
    _80211QOSData(uint32_t rtLen, uint32_t fLen, processServer* );
    ~_80211QOSData() = default;

    inline std::weak_ptr<qos_t> getFrameBody() const;

    static void resetParseFunc(std::function<void(void* args)>);

    void setFrameBody(u_char* data) override;
    void parse() override;

private:
    std::shared_ptr<qos_t> frameBody;
    static std::function<void(void* args)> parseFunc;
};

std::weak_ptr<qos_t> _80211QOSData::getFrameBody() const{
    return std::weak_ptr<qos_t>(frameBody);
}

#endif // 80211PACKET_H
