#include <click/element.hh>
#include <click/etheraddress.hh>
#include <click/ipaddress.hh>
#include <click/sync.hh>
#include <click/timer.hh>

#include "graph.hh"
CLICK_DECLS

class GraphBuilder;

class GraphBuilder: public Element{
public:
	GraphBuilder();
	~GraphBuilder();

	const char *class_name() const{
		return "GraphBuilder";
	}
	const char *port_count() const{
		"";
	}
	const char *processing() const{
		return PUSH;
	}

	int configure(Vector<String> &, ErrorHandler *);
	void broadcast(WritablePacket *);
	void forward(int, Packet *);
	void detect(Timer*);
	//Detect adjacent routers periodically

	void push(int, Packet *);

private:
	Graph *graph;
	Timer _timer_period;
	Timer _timer_delay;
	uint32_t _ip_address;
	uint32_t _time_out;
	uint32_t _delay;
	Vector<String> answers;
	
	HashTable<uint32_t, int> port_table;
	Vector<uint32_t> neighbor;
};

CLICK_ENDDECLS
	
