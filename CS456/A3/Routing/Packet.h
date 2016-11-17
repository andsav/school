#ifndef ROUTING_PACKET_H
#define ROUTING_PACKET_H

#define NBR_ROUTER 5

#include <string>
#include <memory>

#include "State.h"

struct Packet
{
    unsigned int router_id;

    char const* encode() const;

    static std::unique_ptr<Packet> make(std::string&);
    void update_state(State&);
};

struct PacketHello : public Packet
{
    unsigned int link_id;

    char const* encode() const;
};

struct PacketLS : public Packet
{

};

struct PacketInit : public Packet
{

};

struct PacketError : public Packet
{

};


#endif //ROUTING_PACKET_H
