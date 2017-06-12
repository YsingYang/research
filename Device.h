#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <vector>

class deviceSet;
class device{
public:
    device(std::string MAC, short cap, uint32_t pkSize, uint16_t seq); //pksize为除去ssid, radiotap剩余的帧长
    static bool recognizeMAC(const std::string&); //要不设置成静态的?
    static std::string DecToHexStr(const std::string& );

private:
    short capInfo;
    std::string MACKey;
    std::string currentMAC;
    uint32_t packetSize;
    uint16_t sequence;
};


#endif // DEVICE_H
