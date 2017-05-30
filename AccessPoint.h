#ifndef ACCESSPOINT_H
#define ACCESSPOINT_H

#include <vector>
#include "type.h"
class accessPoint{
public:
    accessPoint() = default;
    static void addAccessPoint();
    static std::vector<std::vector<u_char>> APmap;
private:
    accessPoint(accessPoint &) = delete;
    accessPoint& operator= (accessPoint&) = delete;
};


#endif // ACCESSPOINT_H
