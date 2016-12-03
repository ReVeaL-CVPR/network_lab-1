#ifndef CLICK_PACKETHEADER_HH
#define CLICK_PACKETHEADER_HH

#include <click/packet.hh>

typedef enum {
    DATA = 0,
    HELLO,
    ACK,
    UPDATE,
    TCP_HELLO,
    TCP_ACK,
    TCP_DATA
} packet_types;

struct PacketHeader{
     uint8_t type; 
     uint8_t sequence;
     uint8_t source;
     uint8_t destination;
     uint32_t size;
};

#endif
