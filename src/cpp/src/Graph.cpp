#include "Graph.h"

using namespace std;

denselyNumberedGraph::denselyNumberedGraph(sparselyNumberedGraph& g, unordered_map<nodeID, nodeID>& transform) : denselyNumberedGraph(g.nodes()){
    transform.reserve(g.nodes());
    nodeID counter = 0;
    for(auto& pair : g){
        transform[pair.first] = counter++;
    }
    for(auto& pair : g){
        for(auto n : pair.second){
            addEdge(transform[pair.first], transform[n], true); // edges are already directed
        }
    }
    no_edges = g.no_edges;
}

denselyNumberedGraph::denselyNumberedGraph(sparselyNumberedGraph& g){
    unordered_map<nodeID, nodeID> transform;
    transform.reserve(g.nodes());
    nodeID counter = 0;
    for(auto& pair : g){
        transform[pair.first] = counter++;
    }
    for(auto& pair : g){
        for(auto n : pair.second){
            addEdge(transform[pair.first], transform[n], true); // edges are already directed
        }
    }
    no_edges = g.no_edges;
}

sparselyNumberedGraph::sparselyNumberedGraph(denselyNumberedGraph& g) : sparselyNumberedGraph(){
    for(int i=0; i < g.nodes(); i++){
        adj_map[i] = g[i];
    }
    no_edges=g.edges();
}