
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
};
