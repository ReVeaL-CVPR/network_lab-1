struct Edge;

struct Edge{
public:
	uint32_t u, v;
	uint32_t seq;
	bool valid;
	
	Edge(uint32_t _u, uint32_t _v, uint32_t _seq): u(_u), v(_v), seq(_seq), valid(true){
	}
private:
	Edge();
	Edge(Edge &);
	Edge &operator =(const Edge &);
};


class Graph;

class Graph{
public:
	Graph(EtherAddress);
	~Graph();
	
	int try_add_edge(EtherAddress, EtherAddress);
	int try_delete_edge(EtherAddress, EtherAddress);
	EtherAddress nexthop(EtherAddress);
	
	enum {
		E_NODE_NOT_EXIST = 1;
		E_NODE_LIMIT_EXCEEDED;
		
		E_EDGE_NOT_EXIST;
		E_EDGE_EXIST;
	};
		

private:
	uint32_t n;
	static const uint32_t MaxNodeNum = 256;
/*
	List<uint32_t> edge[MaxNodeNum];
*/

	List<Edge> edges;
	List<List<Edge> :: iterator> edge[MaxNodeNum];
	
	uint32_t dis[MaxNodeNum];
	uint32_t nexthop[MaxNodeNum];
	HashTable<EtherAddress, uint32_t> table;
	
	Graph();
	
	int add_node(EtherAddress);
//	int delete_node(uint32_t);
	
	void table_add(EtherAddress, uint32_t);
//	void table_delete(uint32_t);
	
	void solve();
};

