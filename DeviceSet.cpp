#include "DeviceSet.h"
#include "boost/assign.hpp"

deviceSet::deviceSet() :
    DeviceMapping({{std::pair<short, uint32_t>({0, 0 }),  std::set<std::shared_ptr<device>, setCompare>()}}),
    nullIterator(DeviceMapping[std::pair<short, uint32_t>({0, 0})].end()){
    //DeviceMapping[static_cast<deviceKey>({0, 0})] = std::set<std::shared_ptr<device>>;//用于find时返回一个.end() 表示未找到相应元素
}

std::set<std::shared_ptr<device>>::const_iterator deviceSet::findDeviceInSet(const std::shared_ptr<device> &dv){
    deviceKey key(dv->getCapInfo(), dv->getPacketSize());
    auto keyIt = DeviceMapping.find(key);
    if(keyIt == DeviceMapping.end())
            return nullIterator;//没有这个集合, 不存在
    std::string thisMACKey = dv->getMACKey();
    if(randomMapping.find(thisMACKey) != randomMapping.end()){
        return keyIt->second.find(randomMapping[thisMACKey]) //最好加个assert
    }
    return keyIt->second.find(dv);
}
