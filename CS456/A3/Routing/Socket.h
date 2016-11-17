#ifndef ROUTING_SOCKET_H
#define ROUTING_SOCKET_H

#define BUFFER_SIZE 2048

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <memory>

#include "Packet.h"

struct Socket {
    int fd;
    bool initialized;
    sockaddr_in router_address, nse_address;

    Socket();
    ~Socket();

    bool init(int, const std::string&, int);
    bool send(const Packet&);
    std::unique_ptr<Packet> receive();
};

#endif //ROUTING_SOCKET_H
