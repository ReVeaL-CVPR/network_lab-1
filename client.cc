#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/timer.hh>
#include <click/packet.hh>
#include "client.hh" 
#include "packets.hh"

CLICK_DECLS 

BasicClient::BasicClient() : _timerTO(this){
    _seq = 0;
    _period = 3;
    _periodHello = 2;
    _delay = 0;
    _time_out = 1;
    _my_address = 0;
    _other_address = 0;
    transmissions = 0;
}

BasicClient::~BasicClient(){
    
}

int BasicClient::initialize(ErrorHandler *errh){
    _timerTO.initialize(this);
    return 0;
}

int BasicClient::configure(Vector<String> &conf, ErrorHandler *errh) {
  if (cp_va_kparse(conf, this, errh,
                  "MY_ADDRESS", cpkP+cpkM, cpUnsigned, &_my_address,
                  "OTHER_ADDRESS", cpkP+cpkM, cpUnsigned, &_other_address,
                  "DELAY", cpkP, cpUnsigned, &_delay,
                  "PERIOD", cpkP, cpUnsigned, &_period,
                  "TIME_OUT", cpkP, cpUnsigned, &_time_out,
                  cpEnd) < 0) {
    return -1;
  }
  return 0;
}

void BasicClient::run_timer(Timer *timer) {
    if(timer == &_timerTO){
        click_chatter("Retransmitting packet %u for %d time", _seq, transmissions);
        WritablePacket *packet = _wrapper(buff, TCP_DATA, 0, _my_address, 0, strlen(buff));
        output(0).push(packet);
        transmissions++;
        _timerTO.schedule_after_sec(_time_out);
    }else {
        assert(false);
    }
}




void BasicClient::push(int srcprt, Packet *p) {
    assert(p);
    struct PacketHeader *header = (struct PacketHeader *)p->data();
    if(header->type == HELLO){ //data -> send ack
        click_chatter("HELLO received.\n");
        WritablePacket *packet = _wrapper("", ACK, 0, _my_address, header->source, 1);
        output(0).push(packet);
    } 
    else if (header->type == TCP_DATA) 
    { //received ack -> schedule new data packet
        struct PacketHeader *header = (struct PacketHeader *)p->data();
        char* dat = ((char *)header + sizeof(struct PacketHeader))
        click_chatter("Received data %s from %u", dat, header->source);
        WritablePacket *packet = _wrapper("", TCP_ACK, 0, _my_address, header->source, 1);
        output(0).push(packet);
    } 
    else if (header->type == TCP_ACK) 
    { //received ack -> unschedule the data packet
        click_chatter("TCPACK received.\n");
        _timerTO.unschedule();
        transmissions=0;
    } 
    p -> kill();
}


int BasicClient::send(const String &conf, Element *e, void *, ErrorHandler * errh){
    BasicClient *me = (BasicClient *) e;
    uint32_t to;
    if(cp_va_kparse(conf, me, errh,
                     "CONTENT", cpkM, cpString, &buff,
                     "TO", cpkM, cpIPAddress, &to, 
                     cpEnd) < 0)
        return -1;
    WritablePacket *packet = _wrapper(content, TCP_DATA, 0, _my_address, to, strlen(content));
    output(0).push(packet);
    _timerTO.schedule_after_sec(_time_out);
    return 0;
}

void BasicClient::add_handlers() {
    add_write_handler("send", &send, (void *)0);
}

CLICK_ENDDECLS 
EXPORT_ELEMENT(BasicClient)
