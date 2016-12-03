#include <click/config.h>

#include "PacketHeader.hh"

CLICK_DECLS

WritablePacket *_wrapper
(const char* payload, int type, int sequence, int source, int destination, int size){
    WritablePacket *packet = Packet::make(0,0,sizeof(struct PacketHeader) + size, 0);
    memset(packet->data(),0,packet->length());
    struct PacketHeader *format = (struct PacketHeader*) packet->data();
    format->type = type;
    format->sequence = sequence;
    format->source = source;
    format->destination = destination;
    format->size = size;
    char *data = (char*)(packet->data()+sizeof(struct PacketHeader));
//    strcpy(data, payload);
	memcpy(data, payload, size * sizeof(char));
    
    return packet;
}

CLICK_ENDDECLS
