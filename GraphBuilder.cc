#include <click/config.h>
#include <click/confparse.hh>

#include "Graph.hh"
#include "GraphBuilder.hh"

CLICK_DECLS

GraphBuilder::GraphBuilder(): _timer_period(this), _timer_delay(this){
	answers.clear();
	neighbor.clear();
}

GraphBuilder ::
~GraphBuilder(){
}

int GraphBuilder::initialize(ErrorHandler *errh){
    _timer_period.initialize(this);
    _timer_delay.initialize(this);
    _timer_period.schedule_after_sec(_time_out);
    _timer_delay.schedule_after_sec(_delay);
    graph = new Graph(_ip_address);
    return 0;
}


int GraphBuilder :: 
configure(Vector<String> &conf, ErrorHandler *errh) {
  if (cp_va_kparse(conf, this, errh,
                  "MY_ADDRESS", cpkP+cpkM, cpUnsigned, &_ip_address,
			      "DELAY", cpkP, cpUnsigned, &_delay,
			      "TIME_OUT", cpkP, cpUnsigned, &_time_out,
                  cpEnd) < 0) {
    return -1;
  }
  return 0;
}

WritablePacket *GraphBuilder ::
_wrapper(const char* payload, int type, int sequence, int source, int destination, int size){
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

void GraphBuilder::broadcast(WritablePacket *p){
	//TODO traverse all the port and send the packet
	int n = noutputs();
	int sent = 0;
	for (int i = 0; i < n; i++){
		Packet *pp = (sent < n - 1 ? p->clone() : p);
		output(i).push(pp);
		sent++;
     }
}

void GraphBuilder::forward(int src, Packet *p){
	int n = noutputs();
	int sent = 0;
	for (int i = 0; i < n; i++)
	 if (i != src){
		Packet *pp = (sent < n - 2 ? p->clone() : p);
		output(i).push(pp);
		sent++;
     }
} 

void GraphBuilder::run_timer(Timer* timer){
	if(timer == &_timer_period){
//		answers = new Vector<uint32_t>();
		answers.clear();
		ans_port.clear();
		WritablePacket *packet = _wrapper("hello", HELLO, 0, _ip_address, 0, 6);
		broadcast(packet);
		_timer_delay.schedule_after_sec(_delay);
		_timer_period.schedule_after_sec(_time_out);
	}
	else if (timer == &_timer_delay)
	{
		bool isnew = 0;
		Vector<uint32_t> :: iterator it;
		Vector<uint32_t> :: iterator jt;
		Vector<int> :: iterator kt;
		port_table.clear();
		for (it = answers.begin(), kt = ans_port.begin(); it != answers.end(); ++it, ++kt){
			if (*kt >= port_table.size())
				port_table.resize(*kt + 1);
			port_table[*kt] = *it;
			click_chatter("ip %u vs port %d", *it, *kt);
		}
		for(it = answers.begin(); it != answers.end(); ++it){
			bool f = 0;
			for (jt = neighbor.begin(); jt != neighbor.end(); ++jt){
				if (*it == *jt){
					f = 1;
					break;
				}
			}
			if (f == 0){
				click_chatter("add edge %u", _ip_address);
				graph -> try_add_edge(_ip_address, *it);
				isnew = 1;
			}
		}
		for(it = neighbor.begin(); it != neighbor.end(); ++it){
			bool f = 0;
			for (jt = answers.begin(); jt != answers.end(); ++jt){
				if (*it == *jt){
					f = 1;
					break;
				}
			}
			if (f == 0){
				click_chatter("delete edge %u", _ip_address);
				graph -> try_delete_edge(_ip_address, *it);
				isnew = 1;
			}
		}
		if(isnew){
			neighbor = answers;
			graph -> solve();
			Pair<char *, int> payload = graph -> toPayload();
			WritablePacket *packet = _wrapper(payload.first, UPDATE, 0, _ip_address, 0, payload.second);
			broadcast(packet);
			delete [](payload.first);
		}
	} else {
    	assert(false);
	}
}


void GraphBuilder :: push(int srcprt, Packet *p){
	assert(p);
	struct PacketHeader *header = (struct PacketHeader *)p->data();
	if (header->type == DATA){
		uint32_t dstip = graph -> get_next_hop(header -> destination);
		if (dstip != 0){
			int dstprt = table_lookup(dstip);
			if (dstprt >= 0)
				output(dstprt).push(p);
			else
				p -> kill();
		} else
			p -> kill();
	}
	else if (header->type == HELLO){
		WritablePacket *packet = _wrapper("", ACK, 0, _ip_address, header->source, 1);
		p -> kill();
		output(srcprt).push(packet);
	} 
	else if(header->type == ACK) {
		uint32_t from = header->source;
		answers.push_back(from);
		ans_port.push_back(srcprt);
		p -> kill();
	} 
	else if(header->type == UPDATE) {
		click_chatter("UPDATE received.\n");
		bool isnew = 0;
		Edge_transfer *q = (Edge_transfer *)((char *)header + sizeof(struct PacketHeader));
		uint32_t cnt = header -> size / sizeof(Edge_transfer);
		for (int i = 0; i < cnt; ++i){
			if (graph -> check_edge(&q[i]) > 0)
				isnew = 1;
		}
		if (isnew){
			graph -> solve();
			++ header -> sequence;
			forward(srcprt, p);
		}
		else
			p -> kill();		
	}
	else {
		click_chatter("Wrong packet type");
		p->kill();
	}
}

int GraphBuilder ::
table_lookup(uint32_t ea) const{
	Vector<uint32_t> :: const_iterator it;
	for (it = port_table.begin(); it != port_table.end(); ++it)
	 if (*it == ea) return (it - port_table.begin());
	return -1;
}

/*
void GraphBuilder ::
table_insert(uint32_t ea, int port){
	if (port < 0) return ;
	if (port > table.size())
		table.resize(port + 1);
	Vector<uint32_t> :: iterator it;
	for (it = table.begin(); it != table.end(); ++it)
	 if (*it == ea){
		(*it) = ~0u;
		break; 
	 }
	table[port] = ea;
}

void GraphBuilder ::
table_delete(uint32_t ea){
	HashTable<uint32_t, int> :: iterator it;
	it = table.find(ea);
	if (it == table.end())
		return ;
	table.erase(it);
}

void GraphBuilder :: 
*/




CLICK_ENDDECLS
EXPORT_ELEMENT(GraphBuilder)
