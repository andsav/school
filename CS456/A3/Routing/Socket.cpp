#include "Socket.h"

Socket::Socket() {
    this->fd = socket(AF_INET, SOCK_DGRAM, 0);
}

bool Socket::init() {
    memset((void *)&(this->router_address), 0, sizeof(this->router_address));
    this->router_address.sin_family = AF_INET;
    this->router_address.sin_addr.s_addr = htonl(INADDR_ANY);
    this->router_address.sin_port = htons(0);

    unsigned int address_size = sizeof(this->router_address);

    return (bind(fd, (struct sockaddr *)&(this->router_address), sizeof(this->router_address)) >= 0)
        && (getsockname(this->fd, (struct sockaddr *)&(this->router_address), &address_size) >= 0);
}