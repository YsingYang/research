#include "target.h"

pthread_once_t targetControler::once = PTHREAD_ONCE_INIT;//初始化

targetControler* targetControler::instance = nullptr;

targetControler* targetControler::Singleton(){
    pthread_once(&once, &targetControler::init);
    return instance;
}

void targetControler::init(){
    instance = new targetControler();
}

void targetControler::addTarget(const std::vector<char> &target){
    targetList.emplace_back(target);
}
