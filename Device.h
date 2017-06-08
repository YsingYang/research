#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <vector>
class device{
public:
    device(std::string MAC, short cap, uint32_t pkSize, uint16_t seq);
    static bool recognizeMAC(const std::string&); //要不设置成静态的?
    static std::string DecToHexStr(const std::vector<u_char> &addr);

private:
    short capInfo;
    std::string MACKey;
    std::string currentMAC;
    uint32_t packetSize;
    uint16_t sequence;
};


#endif // DEVICE_H
