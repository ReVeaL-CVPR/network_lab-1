#include "Graph.hh"


Graph ::
Graph(uint32_t this_ea): n(0){
	add_node(this_ea);
	
	dis[0] = 0; nexthop[0] = 0;
	
}

Graph ::
~Graph(){
}

/*
int Graph ::
try_add_edge(uint32_t ea1, uint32_t ea2){
	uint32_t n1 = table_lookup(ea1);
	if ((int)n1 < 0) n1 = add_node(ea1);
	if ((int)n1 < 0)
		return -E_NODE_LIMIT_EXCEEDED;
	
	uint32_t n2 = table_lookup(ea2);
	if ((int)n2 < 0) n2 = add_node(ea2);
	if ((int)n2 < 0)
		return -E_NODE_LIMIT_EXCEEDED;
	
	bool exist = 0;
	
	List<uint32_t> :: iterator it_e;
	for (it_e = edge[n1].begin(); it_e != edge[n1].end(); ++it_e)
	 if (*it_e == n2){
		exist = 1;
		break;
	 }
	 
	if (exist) return -E_EDGE_EXIST;
	
	edge[n1].push_back(n2);
	edge[n2].push_back(n1);
	
	solve();
}

int Graph ::
try_delete_edge(uint32_t ea1, uint32_t ea2){
	uint32_t n1 = table_lookup(ea1);
	if ((int)n1 < 0)
		return -E_NODE_NOT_EXIST;
	
	uint32_t n2 = table_lookup(ea2);
	if ((int)n2 < 0)
		return -E_NODE_NOT_EXIST;
	
	bool exist = 0;
	
	List<uint32_t> :: iterator it_e;
	for (it_e = edge[n1].begin(); it_e != edge[n2].end(); ++it_e)
	 if (*it_e == n2){
		exist = 1;
		edge[n1].erase(it_e);
		break;
	 }
	if (!exist)
		return -E_EDGE_NOT_EXIST;
	for (it_e = edge[n2].begin(); it_e != edge[n2].end(); ++it_e)
	 if (*it_e == n1){
		edge[n2].erase(it_e);
		break;
	 }
	
	solve();
	
	return 1;
}

uint32_t Graph ::
get_next_hop(uint32_t ea){
	uint32_t n0 = table_lookup(ea);
	if ((int)n0 < 0)
		return -E_NODE_NOT_EXIST;
	n0 = nexthop[n0];
	return table_rev_lookup(n0);
}

void Graph ::
solve(){
	static bool vis[MaxNodeNum];
	memset(vis, 0, n * sizeof(bool));
	
	static uint32_t Que[MaxNodeNum];
	static uint32_t qh, qt; qh = qt = 0;
	
	vis[0] = 1;
	Que[++qt] = 0;
	dis[0] = 0; nexthop[0] = 0;
	
	while (++qh < qt){
		uint32_t x = Que[qh];
		for (it = edge[x].begin(); it != edge[x].end(); ++it)
		 if (!vis[*it]){
			vis[Que[++qt] = *it] = 1;
			dis[*it] = dis[x] + 1;
			nexthop[*it] = x;
		 }
	}
	
	for (uint32_t i = 0; i < n; ++i)
	 if (node_exist[i] && !vis[i]) delete_node(i);
}

int Graph ::
add_node(uint32_t ea){
	if (n == MaxNodeNum)
		return -E_NODE_LIMIT_EXCEEDED;
	
	uint32_t new_label = n;
	for (int i = 0; i < n; ++i)
	 if (!node_exist[i]){
		 new_label = i;
		 break;
	 }
	n += new_label == n;
	
	table_add(ea, new_label);
}

int Graph ::
delete_node(uint32_t n0){
	if (!node_exist[n0])
		return -E_NODE_NOT_EXIST;
	
	node_exist[n0] = 0;
	
	table_delete(n0);
	
	n -= (n0 + 1 == n);
}
*/

int Graph ::
try_add_edge(uint32_t ea1, uint32_t ea2, uint32_t seq){
	uint32_t n1 = table_lookup(ea1);
	if ((int)n1 < 0) n1 = add_node(ea1);
	if ((int)n1 < 0)
		return -E_NODE_LIMIT_EXCEEDED;
	
	uint32_t n2 = table_lookup(ea2);
	if ((int)n2 < 0) n2 = add_node(ea2);
	if ((int)n2 < 0)
		return -E_NODE_LIMIT_EXCEEDED;
	
	bool exist = 0;
	
	List<Edge> :: iterator it;
	List<List<Edge> :: iterator> :: iterator it1;
	for (it1 = edge[n1].begin(); it1 != edge[n1].end(); ++it1)
	 if ((*it1) -> u == n2 || (*it1) -> v == n2){
		exist = 1;
		break;
	 }
	 
	if (exist){
		it = *it1;
		if (!it -> valid){
			it -> valid = 1;
		} else {
			if (it -> seq < seq)
				it -> seq = seq;
			return -E_EDGE_EXIST;
		}
	} else {
		it = edges.push_back(Edge(n1, n2, seq));
		edge[n1].push_back(it);
		edge[n2].push_back(it);
	}
	
	return 1;
}

int Graph ::
try_add_edge(uint32_t ea1, uint32_t ea2){
	uint32_t n1 = table_lookup(ea1);
	if ((int)n1 < 0) n1 = add_node(ea1);
	if ((int)n1 < 0)
		return -E_NODE_LIMIT_EXCEEDED;
	
	uint32_t n2 = table_lookup(ea2);
	if ((int)n2 < 0) n2 = add_node(ea2);
	if ((int)n2 < 0)
		return -E_NODE_LIMIT_EXCEEDED;
	
	bool exist = 0;
	
	List<Edge> :: iterator it;
	List<List<Edge> :: iterator> :: iterator it1;
	for (it1 = edge[n1].begin(); it1 != edge[n1].end(); ++it1)
	 if ((*it1) -> u == n2 || (*it1) -> v == n2){
		exist = 1;
		break;
	 }
	 
	if (exist){
		it = *it1;
		it -> valid = 1;
		++it -> seq;
	} else {
		it = edges.push_back(Edge(n1, n2, seq));
		edge[n1].push_back(it);
		edge[n2].push_back(it);
	}
	
	return 1;
}

int Graph ::
try_delete_edge(uint32_t ea1, uint32_t ea2){
	uint32_t n1 = table_lookup(ea1);
	if ((int)n1 < 0)
		return -E_NODE_NOT_EXIST;
	
	uint32_t n2 = table_lookup(ea2);
	if ((int)n2 < 0)
		return -E_NODE_NOT_EXIST;
	
	bool exist = 0;
	
	List<List<Edge> :: iterator> :: iterator it;
	for (it = edge[n1].begin(); it != edge[n1].end(); ++it)
	 if (((*it) -> u == n2 || (*it) -> v == n2) && (*it) -> valid){
		(*it) -> valid = 0;
		exist = 1;
		break;
	 }
	if (!exist)
		return -E_EDGE_NOT_EXIST;
	
	return 1;
}

int Graph ::
check_edge(Edge_transfer *et){
	uint32_t u = table_lookup(et -> src);
	if ((int)u < 0) u = add_node(et -> src);
	if ((int)u < 0)
		return -E_NODE_LIMIT_EXCEEDED;
	
	uint32_t v = table_lookup(et -> dst);
	if ((int)v < 0) v = add_node(et -> dst);
	if ((int)v < 0)
		return -E_NODE_LIMIT_EXCEEDED;
	
	int r = 0;
	
	List<Edge> :: iterator it;
	List<List<Edge> :: iterator> :: iterator it1;

	for (it1 = edge[u].begin(); it1 != edge[u].end(); ++it1)
	 if ((*it1) -> v == v || (*it1) -> u == v){
		 it = *it1;
		 if (it -> seq >= et -> seq) break;
		 if (it -> valid == et -> valid) break;
		 r = 1;
		 it -> seq = et -> seq;
		 it -> valid = et -> valid;
		 break;
	 }
	
	if (it1 == edge[u].end()){
		r = 1;
		it = edges.push_back(Edge(u, v, et -> seq, et -> valid));
		edge[u].push_back(it);
		edge[v].push_back(it);
	}
	
	return r;
}	

uint32_t Graph ::
get_next_hop(uint32_t ea){
	uint32_t n0 = table_lookup(ea);
	if ((int)n0 < 0)
		return -E_NODE_NOT_EXIST;
	if (dis[n0] == ~0u >> 1)
		return -E_NODE_CANNOT_REACH;
	n0 = nexthop[n0];
	return table_rev_lookup(n0);
}

Pair<char *, int> Graph ::
toPayload(){
	int s = edges.size() * sizeof(Edge_transfer);
	char *r = new char[s];
	Edge_transfer *ir = (Edge_transfer *)r;
	
	List<Edge> :: iterator it;
	for (it = edges.begin(); it != edges.end(); ++it, ++ir){
		ir -> src = table_rev_lookup(it -> u);
		ir -> dst = table_rev_lookup(it -> v);
		ir -> seq = it -> seq;
		ir -> valid = it -> valid;
	}
	
	return Pair<char *, int>(r, s);
}

void Graph ::
solve(){
	static bool vis[MaxNodeNum];
	memset(vis, 0, n * sizeof(bool));
	
	static uint32_t Que[MaxNodeNum];
	static uint32_t qh, qt; qh = qt = 0;
	
	vis[0] = 1;
	Que[++qt] = 0;
	dis[0] = 0; nexthop[0] = 0;
	
	while (++qh <= qt){
		uint32_t x = Que[qh], y;
		for (List<List<Edge> :: iterator> :: iterator it = edge[x].begin(); it != edge[x].end(); ++it)
		 if ((*it) -> valid){
			y = (*it) -> u + (*it) -> v - x;
			if (!vis[y]){
				vis[Que[++qt] = y] = 1;
				dis[y] = dis[x] + 1;
				nexthop[y] = x;
			}
		 }
	}
/*
	for (uint32_t i = 0; i < n; ++i)
	 if (node_exist[i] && !vis[i]) delete_node(i);
*/
	for (uint32_t i = 0; i < n; ++i)
	 if (!vis[i]){
		 dis[i] = ~0u >> 1;
		 nexthop[i] = 0;
	 }
}

int Graph ::
add_node(uint32_t ea){
	if (n == MaxNodeNum)
		return -E_NODE_LIMIT_EXCEEDED;
	
	table_add(ea, n++);
}

/*
int Graph ::
delete_node(uint32_t n0){
	if (!node_exist[n0])
		return -E_NODE_NOT_EXIST;
	
	node_exist[n0] = 0;
	
	table_delete(n0);
	
	n -= (n0 + 1 == n);
}
*/
void Graph ::
table_add(uint32_t ea, uint32_t label){
	table.set(ea, label);
}
/*
void Graph ::
table_delete(uint32_t label){
	HashTable<uint32_t, uint32_t> :: iterator it;
	for (it = table.begin(); it != table.end(); ++it)
	 if (it->value() == label){
		 table.erase(it);
		 break;
	}
} 
*/
int Graph ::
table_lookup(uint32_t ea){
	HashTable<uint32_t, uint32_t> :: iterator it;
	it = table.find(ea);
	if (it == table.end())
		return -E_NODE_NOT_EXIST;
	return it -> label;
}

int Graph ::
table_rev_lookup(uint32_t label){
	HashTable<uint32_t, uint32_t> :: iterator it;
	for (it = table.begin(); it != table.end(); ++it)
	 if (it -> value() == label)
		return it -> key();
	return -E_NODE_NOT_EXIST;
}
