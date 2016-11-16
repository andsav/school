#ifndef ROUTING_PACKET_H
#define ROUTING_PACKET_H

#define NBR_ROUTER 5

struct Packet
{
    unsigned int router_id;
};

struct PacketHello : public Packet
{

};

struct PacketLSPDU : public Packet
{

};

struct PacketInit : public Packet
{

};



#endif //ROUTING_PACKET_H
