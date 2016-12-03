#ifndef CLICK_GRAPHBUILDER_HH
#define CLICK_GRAPHBUILDER_HH

#include <click/packet.hh>
#include <click/element.hh>
#include <click/etheraddress.hh>
#include <click/ipaddress.hh>
#include <click/timer.hh>

#include <click/vector.hh>

#include "Graph.hh"
#include "PacketHeader.hh"

CLICK_DECLS
class GraphBuilder: public Element{
public:
	GraphBuilder();
	~GraphBuilder();

	const char *class_name() const { return "GraphBuilder"; }
	const char *port_count() const { return "1-/1-"; }
	const char *processing() const { return PUSH; }

	int configure(Vector<String> &, ErrorHandler *);
	void broadcast(WritablePacket *);
	void forward(int, Packet *);
	void detect(Timer*);
	WritablePacket *_wrapper(const char *, int, int, int, int, int);
	//Detect adjacent routers periodically

	void push(int, Packet *);

private:
	Graph *graph;
	Timer _timer_period;
	Timer _timer_delay;
	uint32_t _ip_address;
	uint32_t _time_out;
	uint32_t _delay;
	Vector<uint32_t> answers;

	Vector<uint32_t> neighbor;
};
CLICK_ENDDECLS

#endif

