#include "Socket.h"

Socket::Socket() {
    this->fd = socket(AF_INET, SOCK_DGRAM, 0);
}

bool Socket::init(int router_port, const std::string& nse_address, int nse_port) {
    // Get nse host's IP address
    hostent *hp = gethostbyname(nse_address.c_str());

    if(!hp)
        return false;

    // Fill router's address
    memset((void *)&(this->router_address), 0, sizeof(this->router_address));
    this->router_address.sin_family = AF_INET;
    this->router_address.sin_addr.s_addr = htonl(INADDR_ANY);
    this->router_address.sin_port = htons(router_port);

    // Fill nse server's address
    memset((char*)&(this->nse_address), 0, sizeof(this->nse_address));
    this->nse_address.sin_family = AF_INET;
    this->nse_address.sin_port = htons(nse_port);
    memcpy((void *)&(this->nse_address).sin_addr, hp->h_addr_list[0], hp->h_length);


    unsigned int address_size = sizeof(this->router_address);
    return (bind(fd, (struct sockaddr *)&(this->router_address), sizeof(this->router_address)) >= 0)
        && (getsockname(this->fd, (struct sockaddr *)&(this->router_address), &address_size) >= 0);
}

bool Socket::send(const Packet& p) {
    char const* msg = p.encode();
    return (sendto(this->fd, msg, strlen(msg), 0, (struct sockaddr *)&(this->nse_address), sizeof(this->nse_address)) >= 0);
}

Packet* Socket::receive() {
    return Packet::make("");
}