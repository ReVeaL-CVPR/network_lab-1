#ifndef CLICK_GRAPH_HH
#define CLICK_GRAPH_HH

#include <click/config.h>
#include <click/element.hh>
#include <click/pair.hh>
#include <click/vector.hh>
#include <click/hashtable.hh>

CLICK_DECLS

struct Edge{
public:
	uint32_t u, v;
	uint32_t seq;
	bool valid;
	
	Edge(const Edge &that): u(that.u), v(that.v), seq(that.seq), valid(that.valid){
	}
	Edge &operator =(const Edge &that){
		u = that.u; v = that.v;
		seq = that.seq;
		valid = that.valid;
		return *this;
	}
	Edge(uint32_t _u, uint32_t _v, uint32_t _seq, bool _valid = 1): u(_u), v(_v), seq(_seq), valid(_valid){
	}
private:
	Edge();

};

struct Edge_transfer{
	uint32_t src, dst;
	uint32_t seq;
	bool valid;
};

class Graph: public Element{
public:
	Graph();
	Graph(uint32_t);
	~Graph();
	
	const char *class_name() const { return "Graph"; };
	const char *port_count() const { return "1-/1-"; };
	const char *processing() const { return PUSH; };
	
	//int configure(Vector<String> &, ErrorHandler *);
	
	int try_add_edge(uint32_t, uint32_t);
	int try_add_edge(uint32_t, uint32_t, uint32_t);
	int try_delete_edge(uint32_t, uint32_t);
	int check_edge(Edge_transfer *);
	uint32_t get_next_hop(uint32_t) const;
	void solve();
	
	Pair<char *, int> toPayload() const;
	
	typedef enum {
		E_NODE_NOT_EXIST = 1,
		E_NODE_LIMIT_EXCEEDED,
		
		E_EDGE_NOT_EXIST,
		E_EDGE_EXIST,
		
		E_NODE_CANNOT_REACH
	} ErrorCode_Type;
		

private:
	uint32_t n;
	static const uint32_t MaxNodeNum = 256;
/*
	Vector<uint32_t> edge[MaxNodeNum];
*/

	Vector<Edge> edges;
	Vector<Vector<Edge> :: iterator> edge[MaxNodeNum];
	
	uint32_t dis[MaxNodeNum];
	uint32_t nexthop[MaxNodeNum];
	HashTable<uint32_t, uint32_t> table;
	
	Graph(const Graph &);
	Graph &operator =(const Graph &);
	
	int add_node(uint32_t);
//	int delete_node(uint32_t);
	
	void table_add(uint32_t, uint32_t);
//	void table_delete(uint32_t);
	int table_lookup(uint32_t) const;
	int table_rev_lookup(uint32_t) const;
};

CLICK_ENDDECLS
#endif
