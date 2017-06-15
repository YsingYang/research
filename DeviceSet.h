#ifndef DEVICESET_H
#define DEVICESET_H

#include <map>
#include <memory>
#include <set>
class device;

class deviceSet{
public:
    deviceSet() = default;
    ~deviceSet() = default;

    void matching();
    void addDevice(std::shared_ptr<device> );
    void updateDevice(std::shared_ptr<device>);
    void removeDevice(std::string );

private:
    deviceSet(deviceSet &) = delete;
    deviceSet& operator=(deviceSet &) = delete;

    std::map<std::string, std::shared_ptr<device>> capturedDeviceSet;
    std::map<std::pair<int, int>, std::set<std::shared_ptr<device>>> DeviceMapping;
};


#endif // DEVICESET_H
