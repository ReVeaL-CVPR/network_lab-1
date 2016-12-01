#include <click/packet.hh>

typedef enum {
    DATA = 0,
    HELLO,
    ACK,
    UPDATE
} packet_types;

struct PacketHeader{
     uint8_t type; 
     uint8_t sequence;
     uint8_t source;
     uint8_t destination;
     uint32_t size;
};

public static void wrapper
(char* payload, int type=0, int sequence, int source, int destination){
    WritablePacket *packet = Packet::make(0,0,sizeof(struct PacketHeader) + struct PacketHeader *format = (struct PacketHeader*) packet->data();
    memset(packet->data(),0,packet->length());
    struct PacketHeader *format = (struct PacketHeader*) packet->data();
    format->type = type;
    format->sequence = sequence;
    format->source = source;
    format->destination = destination;
    format->size = packet->length();
    char *data = (char*)(packet->data()+sizeof(struct PacketHeader));
    strcpy(data, payload);
    return packet;
}