#include "Graph.hh"


Graph ::
Graph(EtherAddress this_ea): n(1){
	memset(node_exist, 0, MaxNodeNum * sizeof(bool));
	
	node_exist[0] = 1;
	dis[0] = 0; nexthop[0] = 0;
	table.set(this_ea, 0);
}

Graph ::
~Graph(){
}

int Graph ::
try_add_edge(EtherAddress ea1, EtherAddress ea2){
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
try_delete_edge(EtherAddress ea1, EtherAddress ea2){
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

EtherAddress Graph ::
get_next_hop(EtherAddress ea){
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
add_node(EtherAddress ea){
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

void Graph ::
table_add(EtherAddress ea, uint32_t label){
	table.set(ea, label);
}

void Graph ::
table_delete(uint32_t label){
	HashTable<EtherAddress, uint32_t> :: iterator it;
	for (it = table.begin(); it != table.end(); ++it)
	 if (it->value() == label){
		 table.erase(it);
		 break;
	}
} 

int Graph ::
table_lookup(EtherAddress ea){
	HashTable<EtherAddress, uint32_t> :: iterator it;
	it = table.find(ea);
	if (it == table.end())
		return -E_NODE_NOT_EXIST;
	return it -> label;
}

void Graph ::
table_rev_lookup(uint32_t label){
	HashTable<EtherAddress, uint32_t> :: iterator it;
	for (it = table.begin(); it != table.end(); ++it)
	 if (it -> value() == label)
		return it -> key();
	return EtherAddress();
}
