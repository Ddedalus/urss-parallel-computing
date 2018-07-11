#include "Graph.h"


using namespace std;

// constructors:

vecGraph::vecGraph(mapGraph &g, unordered_map<nodeID, nodeID> &transform) : vecGraph(g.nodes())
{
	transform.reserve(g.nodes());
	nodeID counter = 0;
	for (auto &pair : g)
	{
		transform[pair.first] = counter++;
	}
	for (auto &pair : g)
	{
		for (auto n : pair.second)
		{
			addEdge(transform[pair.first], transform[n], true); // edges are already directed
		}
	}
	no_edges = g.no_edges;
}

vecGraph::vecGraph(mapGraph &g)
{
	unordered_map<nodeID, nodeID> transform;
	transform.reserve(g.nodes());
	nodeID counter = 0;
	for (auto &pair : g)
	{
		transform[pair.first] = counter++;
	}
	for (auto &pair : g)
	{
		for (auto n : pair.second)
		{
			addEdge(transform[pair.first], transform[n], true); // edges are already directed
		}
	}
	no_edges = g.no_edges;
}

mapGraph::mapGraph(vecGraph &g) : mapGraph()
{
	for (int i = 0; i < g.nodes(); i++)
	{
		adj_map[i] = g[i];
	}
	no_edges = g.edges();
}

// vecGraph member functions:

void vecGraph::addEdge(nodeID from, nodeID to, bool directed)
{
	auto max = from > to ? from : to;
	if (nodes() <= max)
		adj_list.resize(max + 1);
	adj_list[from].push_back(to);
	if (!directed)
		adj_list[to].push_back(from);
	no_edges++;
}

// mapGraph member functions:

nodeID mapGraph::quickRemoveEdge(Node &neigh, uint which)
{
	auto temp = neigh.at(which); // range check just in case
	neigh[which] = neigh.back();
	neigh.pop_back();
	no_edges--; // TODO: fix for undirected
	return temp;
}

void mapGraph::addEdge(nodeID from, nodeID to, bool directed)
{
	adj_map[from].push_back(to);
	if (!directed)
		adj_map[to].push_back(from);
	erase_mark = 0;
	no_edges++;
}

void mapGraph::eraseMarked()
{
	if (erase_mark == 0)
		return;
	for (auto &pair : adj_map)
	{
		for (auto index = 0; index < pair.second.size(); index++)
		{
			if (pair.second[index] == erase_mark)
				quickRemoveEdge(pair.second, index--);
		}
	}
	for (auto pair = adj_map.begin(); pair != adj_map.end();)
	{
		if (pair->second.size() == 0)
			pair = adj_map.erase(pair); // node in a tail, remov
		else
			++pair;
	}
}

// printing

ostream &operator<<(ostream &os, Graph &g)
{
	os << "Graph; nodes:" << g.nodes() << " edges:" << g.edges() << endl;
	for (int i = 0; i < 5; i++){
		const auto vc = g[i];
		os << i << ": " << vc << endl;
	}
	if(g.nodes() > 5)
		os << "6: ... ("<<g.nodes()-5<<" other nodes)" <<endl;
	return os;
}