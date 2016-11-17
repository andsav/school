#include "Socket.h"

Socket::Socket() {
    this->fd = socket(AF_INET, SOCK_DGRAM, 0);
}

Socket::~Socket() {
    close(this->fd);
}

/**
 * Initialize Socket
 * Returns true if successful, false otherwise
 */
bool Socket::init(int router_port, const std::string &nse_address, int nse_port) {
    // Skip if already initialized
    if(this->initialized) {
        return true;
    }

    // Get nse host's IP address
    hostent *hp = gethostbyname(nse_address.c_str());

    // Cannot get nse's address: abort and return false
    if (!hp) {
        this->initialized = false;
        return false;
    }

    // Fill router's address
    memset((void *) &(this->router_address), 0, sizeof(this->router_address));
    this->router_address.sin_family = AF_INET;
    this->router_address.sin_addr.s_addr = htonl(INADDR_ANY);
    this->router_address.sin_port = htons(router_port);

    // Fill nse server's address
    memset((char *) &(this->nse_address), 0, sizeof(this->nse_address));
    this->nse_address.sin_family = AF_INET;
    this->nse_address.sin_port = htons(nse_port);
    memcpy((void *) &(this->nse_address).sin_addr, hp->h_addr_list[0], hp->h_length);

    // Attempt to bind and get socket name. If the C socket library returns an error code, return false
    unsigned int address_size = sizeof(this->router_address);

    this->initialized =
                (bind(this->fd, (struct sockaddr *) &(this->router_address), sizeof(this->router_address)) >= 0) &&
                (getsockname(this->fd, (struct sockaddr *) &(this->router_address), &address_size) >= 0);

    return this->initialized;
}

/**
 * Send a packet to the remote address
 * Returns true if successful, false otherwise
 */
bool Socket::send(const Packet &p) {
    if(!this->initialized) {
        return false;
    }

    char const *msg = p.encode();
    return (sendto(this->fd, msg, strlen(msg), 0, (struct sockaddr *) &(this->nse_address),
                   sizeof(this->nse_address)) >= 0);
}

/**
 * Listen for incoming packet
 * Returns a smart pointer to the received packet when received
 */
std::unique_ptr<Packet> Socket::receive() {
    if(!this->initialized) {
        std::string err = "ERROR";
        return Packet::make(err);
    }

    char buffer[BUFFER_SIZE];
    int receive_length;
    socklen_t address_length = sizeof(this->nse_address);

    // Listen until packet is received (receive_length > 0)
    while (1) {
        receive_length = recvfrom(this->fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &(this->nse_address),
                                  &address_length);

        if (receive_length > 0) {
            std::string data = std::string(buffer, receive_length);
            return Packet::make(data);
        }
    }

}