#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <vector>
#include <set>

class deviceSet;
class device{
public:
    device(std::string MAC, short cap, uint32_t pkSize, uint16_t seq, std::set<std::string> ssid); //pksize为除去ssid, radiotap剩余的帧长
    static bool recognizeMAC(const std::string&); //要不设置成静态的?
    static std::string DecToHexStr(const std::string& );

public:
    //一些获取借口的函数
    inline short getCapInfo() const;
    inline std::string getMACKey() const;
    inline std::string getCurrentMAC() const;
    inline uint32_t getPacketSize() const;
    inline uint16_t getSeq() const;
    inline std::set<std::string> getSSIDList() const;

private:
    short capInfo;
    std::string MACKey;
    std::string currentMAC;
    uint32_t packetSize;
    uint16_t sequence;
    std::set<std::string> SSIDList;
};


short device::getCapInfo() const{
    return capInfo;
}

std::string device::getMACKey() const{
    return MACKey;
}

std::string device::getCurrentMAC() const{
    return currentMAC;
}

uint32_t device::getPacketSize() const{
    return packetSize;
}

uint16_t device::getSeq() const{
    return sequence;
}

std::set<std::string> device::getSSIDList() const{
    return SSIDList;
}

#endif // DEVICE_H
