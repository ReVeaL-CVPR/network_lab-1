
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
	
	void push(int, Packet *);
	Packet *pull(int);
	Packet *simple_action(Packet *);

private:
	Graph *graph;
};

CLICK_ENDDECLS
	
