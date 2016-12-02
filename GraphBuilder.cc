#include "GraphBuilder.hh"


GraphBuilder ::
GraphBuilder(){
}

GraphBuilder ::
~GraphBuilder(){
}

int GraphBuilder :: 
configure(Vector<String> &conf, ErrorHandler *errh){
	
}

int BasicClient::configure(Vector<String> &conf, ErrorHandler *errh) {
  if (cp_va_kparse(conf, this, errh,
                  "MY_ADDRESS", cpkP+cpkM, cpUnsigned, &_ip_address,
			      "DELAY", cpkP, cpUnsigned, &_delay,
			      "TIME_OUT", cpkP, cpUnsigned, &_time_out,
                  cpEnd) < 0) {
    return -1;
  }
  return 0;
}

void GraphBuilder::broadcast(WritablePacket *packet){
	//TODO traverse all the port and send the packet
	
	int n = noutputs();
	int sent = 0;
	for (int i = 0; i < n; i++){
		Packet *pp = (sent < n - 1 ? p->clone() : p);
		output(i).push(pp);
		sent++;
     }
}

void GraphBuilder::forward(int src, Packet *packet){
	int n = noutputs();
	int sent = 0;
	for (int i = 0; i < n; i++)
	 if (i != src){
		Packet *pp = (sent < n - 2 ? p->clone() : p);
		output(i).push(pp);
		sent++;
     }
} 

void GraphBuilder::detect(Timer* timer){
	if(timer == &_timer_period){
		click_chatter("Sending hello");
		answers = new Vector<uint32_t>();
		WritablePacket *packet = wrapper("hello", HELLO, _ip_address, 0, 6);
		broadcast(packet);
		_timer_period.schedule_after_sec(_time_out);
	}
	else if (timer == &_timer_delay){
		bool isnew = 0;
		Vector<uint32_t> :: iterator it;
		Vector<uint32_t> :: iterator jt;
		for(it = answers.begin(); it != answers.end(); ++it){
			//TODO construct the new neighbor info
			bool f = 0;
			for (jt = neighbor.begin(); jt != neighbor.end(); ++jt){
				if (*it == *jt){
					f = 1;
					break;
				}
			}
			if (f == 0){
				graph -> try_add_edge(_ip_address, *it);
				isnew = 1;
			}
		}
		
		for(it = neighbor.begin(); it != neighbor.end(); ++it){
			//TODO construct the new neighbor info
			bool f = 0;
			for (jt = answers.begin(); jt != answers.end(); ++jt){
				if (*it == *jt){
					f = 1;
					break;
				}
			}
			if (f == 0){
				graph -> try_delete_edge(_ip_address, *it);
				isnew = 1;
			}
		}
		
		if(isnew){
			graph -> solve();
			Pair<char *, int> payload = graph -> toPayload();
			WritablePacket *packet = wrapper(payload.first, UPDATE, 0, _ip_address, 0, payload.second);
			broadcast(packet);
		}
	}
}


void push(int srcprt, Packet *p){
	assert(packet);
	struct PacketHeader *header = (struct PacketHeader *)packet->data();
	if(header->type == ACK) {
		uint32_t from = header->source;
		answers.add(from);
	} 
	else if(header->type == UPDATE) {
		bool isnew = 0;
		Edge_transfer *q = (Edge_transfer *)((char *)header + sizeof(struct PacketHeader));
		uint32_t cnt = header -> size / sizeof(Edge_transfer);
		for (int i = 0; i < cnt; ++i){
			if (graph -> check_edge(q[i]) > 0)
				isnew = 1;
		}
		if (isnew){
			graph -> solve();
			forward(srcprt, p);
		}
		
	} 
	else {
		click_chatter("Wrong packet type");
		packet->kill();
	}
}
