#include "Packet.h"

Packet* Packet::make(char const* data)
{
    return new PacketHello();
}

char const* Packet::encode() const {
    char const *p = "abc";
    return p;
}

char const* PacketHello::encode() const {
    char const *p = "abc";
    return p;
}
