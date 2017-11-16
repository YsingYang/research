#ifndef DEVICESET_H
#define DEVICESET_H

#include <map>
#include <memory>
#include <set>
#include "Device.h"
class device;
struct sameKeyDevice;

struct setCompare : public std::binary_function<std::shared_ptr<device>, std::shared_ptr<device>, bool>{
    bool operator()(const std::shared_ptr<device>& d1, const std::shared_ptr<device> &d2) const{
        return d1->getMACKey() < d2->getMACKey();
    }
}; //以MAC作为比较操作的set


class deviceSet{
public:
    typedef std::pair<short, uint32_t> deviceKey;
    typedef std::set<std::shared_ptr<device>, setCompare>::iterator deviceIterator;
    typedef std::set<std::shared_ptr<device>, setCompare> SetType;

    deviceSet();
    ~deviceSet() = default;

    void matching();
    void addDevice(std::shared_ptr<device> );
    void updateDevice(std::shared_ptr<device>);
    void removeDevice(std::string );
    std::set<std::shared_ptr<device>, setCompare>::iterator findDeviceInSet(const std::shared_ptr<device>&);
    inline bool isNulliterator(deviceIterator it);
private:
    deviceSet(deviceSet &) = delete;
    deviceSet& operator=(deviceSet &) = delete;

    std::map<std::pair<short, uint32_t>, std::set<std::shared_ptr<device>, setCompare>> DeviceMapping;
    std::map<std::string, std::shared_ptr<device>> randomMapping;
    const std::set<std::shared_ptr<device>, setCompare>::iterator nullIterator;
};




bool deviceSet::isNulliterator(deviceIterator it){
    return it ==nullIterator;
}



#endif // DEVICESET_H
