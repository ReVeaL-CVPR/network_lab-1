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

void push(int srcprt, Packet *p){
	if (srcprt != PORT){
		p -> kill();
		return ;
	}
	
	click_ether *e = (click_ether *)p->data();
	if (respond){
		EtherAddress dst(e->ether_dhost);
		if (graph -> try_add_edge(local_ether, dst) < 0){
			p -> kill();
			return ;
		}
	}
	//modify
	//forward information
}




