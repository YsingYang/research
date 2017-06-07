#ifndef TARGET_H
#define TARGET_H

#include <pthread.h>
#include <vector>

//单例
class targetControler{
public:
    static targetControler* Singleton();
    inline std::vector<std::vector<char>>& getTargetList();
    ~targetControler() = default;
    void addTarget(const std::vector<char> &target);

private:
    targetControler() = default;
    targetControler(targetControler&) = delete;
    targetControler& operator=(targetControler&) = delete;
    static void init();

private:
    std::vector<std::vector<char>> targetList;
    static targetControler* instance;
    static pthread_once_t once;
};

std::vector<std::vector<char>>& targetControler::getTargetList(){
    return targetList;
}


#endif // TARGET_H
