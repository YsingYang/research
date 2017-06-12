#include "../DeviceSet.h"
#include "../Device.h"
#include <iostream>

using namespace std;

int main(){
    std::string addr = "\x02\x02\x34\x42\x23\x00";
    string res = device::DecToHexStr(addr);
    cout<<res<<boolalpha<<device::recognizeMAC(res)<<endl;
    return 0;
}
