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
}

void GraphBuilder::detect(Timer* timer){
	if(timer == &_timer_period){
		click_chatter("Sending hello");
		answers = new Vector<uint32_t>();
		WritablePacket *packet = wrapper("hello", HELLO, _ip_address, 0);
		broadcast(packet);
		_timer_period.schedule_after_sec(_time_out);
	}
	else if (timer == &_timer_delay){
		Vector<uint32_t> :: iterator it;
		for(it = answers.begin(); it != answers.end(); ++it){
			//TODO construct the new neighbor info
		}
		if(//TODO if neighbor info differ from the current table
			){
			//TODO update
			WritablePacket *packet = wrapper("", UPDATE, _ip_address, 0);
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
		//TODO check if the update information is new
		if (isnew){
			//TODO update
			broadcast(p);
		}
	} 
	else {
		click_chatter("Wrong packet type");
		packet->kill();
	}

	// if (srcprt != PORT){
	// 	p -> kill();
	// 	return ;
	// }
	
	// click_ether *e = (click_ether *)p->data();
	// if (respond){
	// 	EtherAddress dst(e->ether_dhost);
	// 	if (graph -> try_add_edge(local_ether, dst) < 0){
	// 		p -> kill();
	// 		return ;
	// 	}
	// }
	//modify
	//forward information
}
