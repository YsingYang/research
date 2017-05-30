#include "pcapHandle.h"
#include "processServer.h"
#include "AccessPoint.h"


int main(){
    //char buff[1024];
    //printf("%s", pcap_lookupdev(buff));
    accessPoint::addAccessPoint();
    processServer *server = processServer::Singleton();
    server->serverProcess();

}
