#ifndef DEVICESET_H
#define DEVICESET_H

#include <map>
#include <memory>
#include <set>
#include "Device.h"
class device;
struct sameKeyDevice;
struct setCompare ;

class deviceSet{
    typedef std::pair<short, uint32_t> deviceKey;
public:
    deviceSet() = default;
    ~deviceSet() = default;

    void matching();
    void addDevice(std::shared_ptr<device> );
    void updateDevice(std::shared_ptr<device>);
    void removeDevice(std::string );
    inline bool deviceInSet(const std::shared_ptr<device>&);

private:
    deviceSet(deviceSet &) = delete;
    deviceSet& operator=(deviceSet &) = delete;

    std::map<std::string, std::shared_ptr<device>> capturedDeviceSet;
    std::map<std::pair<short, uint32_t>, std::set<std::shared_ptr<device>, setCompare>> DeviceMapping;
};

struct setCompare : public std::binary_function<std::shared_ptr<device>, std::shared_ptr<device>, bool>{
    bool operator()(const std::shared_ptr<device>& d1, const std::shared_ptr<device> &d2) const{
        return d1->getMACKey() < d2->getMACKey();
    }
}; //以MAC作为比较操作的set

/*
struct deviceKey{
    device(short cap, uint32_t pk, std::string MAC):capInfo(cap), packetSize(pk), MACKey(MAC){}
    device(const deviceKey &key){
        capInfo = key.info;
        packetSize = key.packetSize;
        MACKey = key.MACKey;
    }

    ~device() = default;
    bool operator==(const deviceKey& key){
        return capInfo == key.capInfo && packetSize == key.packetSize && MACKey == key.MACKey;
    }

    bool operator<(const deviceKey& key){
        if(capInfo == key.capInfo){
            if(packetSize == key.packetSize){
                return MACKey < key.MACKey;
            }
            return packetSize < key.packetSize;
        }
        return capInfo < key.capInfo;
    }

    short capInfo;
    uint32_t packetSize;
    std::string MACKey;
};

struct sameKeyDevice{
    sameKeyDevice(std::string MAC, std::shared_ptr<device> device): MACKey(MAC), device_(device){}
    sameKeyDevice(const sameKeyDevice& dv){
        MACKey = dv.MACKey;
        device_ = dv.device_;
    }
    sameKeyDevice& operator=(const sameKeyDevice& dv){
        MACKey = dv.MACKey;
        device_ = dv.device_;
        return *this;
    }

    bool operator==(const sameKeyDevice& dv){
        return MACKey == dv.MACKey;
    }

    bool operator <(const sameKeyDevice& dv){
        return MACKey < dv.MACKey;
    }

std::string MACKey;
std::shared_ptr<device> device_;
};*/

bool deviceSet::deviceInSet(const std::shared_ptr<device> &dv){
    deviceKey key(dv->getCapInfo(), dv->getPacketSize());
    auto keyIt = DeviceMapping.find(key);
    if(keyIt == DeviceMapping.end())
            return false;//没有这个集合, 不存在
    return keyIt->second.find(dv) == keyIt->second.end();
}


#endif // DEVICESET_H
