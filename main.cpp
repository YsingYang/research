#include "pcapHandle.h"
#include "processServer.h"

int main(){
    //char buff[1024];
    //printf("%s", pcap_lookupdev(buff));
    processServer *server = processServer::Singleton();
    server->serverProcess();

}
