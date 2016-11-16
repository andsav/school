#ifndef ROUTING_PACKET_H
#define ROUTING_PACKET_H

#define NBR_ROUTER 5

struct Packet
{
    unsigned int router_id;

    char const* encode() const;

    static Packet* make(const char*);
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



#endif //ROUTING_PACKET_H
