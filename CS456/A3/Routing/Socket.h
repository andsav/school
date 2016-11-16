#ifndef ROUTING_SOCKET_H
#define ROUTING_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

struct Socket {
    int fd;
    sockaddr_in router_address;

    Socket();
    bool init();
};

#endif //ROUTING_SOCKET_H
