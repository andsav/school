#ifndef ROUTING_SOCKET_H
#define ROUTING_SOCKET_H

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

#include "Packet.h"

//using namespace std;

struct Socket {
    int fd;
    sockaddr_in router_address, nse_address;

    Socket();
    bool init(int, const std::string&, int);
    bool send(const Packet&);
    Packet* receive();
};

#endif //ROUTING_SOCKET_H
