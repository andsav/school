#include "Packet.h"

std::unique_ptr<Packet> Packet::make(std::string& data) {
    std::unique_ptr<Packet> p(new Packet);
    return p;
}

void Packet::update_state(State& state) {

}

char const* Packet::encode() const {
    char const *p = "abc";
    return p;
}

char const* PacketHello::encode() const {
    char const *p = "abc";
    return p;
}
