#ifndef __GRAPH_CLASS_H__
#define __GRAPH_CLASS_H__

#include "public.h"


using nodeID = uint;
using Node = std::vector<nodeID>;
// if needed, vector property table may be added later.
class Graph
{
protected:
    uint no_edges;
    /* data */
public:
    uint edges(){return no_edges;}
    virtual uint nodes() = 0;
    virtual std::vector<nodeID>& operator[](nodeID pos) = 0;    // just get adjacency pair
};
class sparselyNumberedGraph;
class denselyNumberedGraph : public Graph{
private:
    std::vector<Node> adj_list;
public:
    denselyNumberedGraph(uint size) : Graph(), adj_list(size){
        no_edges = 0;
    }
    denselyNumberedGraph(sparselyNumberedGraph& g, std::unordered_map<nodeID, nodeID>& transform);
    denselyNumberedGraph(sparselyNumberedGraph& g);
    uint nodes() override {   return adj_list.size();  }
    std::vector<nodeID>& operator[](nodeID pos) override {
        return adj_list[pos];
    }
    void addEdge(nodeID from, nodeID to, bool directed = false){
        adj_list[from].push_back(to);
        if(!directed) adj_list[to].push_back(from);
        no_edges++;
    }
    std::vector<Node>& data(){  return adj_list;    }

    // range based for loop support
    std::vector<Node>::iterator begin(){
        return adj_list.begin();
    }
    std::vector<Node>::iterator end(){
        return adj_list.end();
    }

    sparselyNumberedGraph& sparselyNumberedCopy();
};

class sparselyNumberedGraph : public Graph {
friend denselyNumberedGraph;
private:
    std::unordered_map<nodeID, Node> adj_map;
    uint erase_mark;    // easy bulk removals enabled if this is not 0

    nodeID quickRemoveEdge(Node& neigh, uint which){
        auto temp = neigh.at(which);    // range check just in case
        neigh[which] = neigh.back();
        neigh.pop_back();
        no_edges--; // TODO: fix for undirected
        return temp;
    }
public:
    sparselyNumberedGraph() : Graph(), erase_mark(0){}
    sparselyNumberedGraph(denselyNumberedGraph& g);
    uint nodes() override {   return adj_map.size();}
    std::vector<nodeID>& operator[](nodeID pos) override {
        return adj_map[pos];
    }
    void addEdge(nodeID from, nodeID to, bool directed = false){
        adj_map[from].push_back(to);
        if(!directed) adj_map[to].push_back(from);
        erase_mark = 0;
        no_edges++;
    }
    std::unordered_map<nodeID, Node>& data(){    return adj_map; }

    // range based for loop support
    std::unordered_map<nodeID, Node>::iterator begin(){
        return adj_map.begin();
    }
    std::unordered_map<nodeID, Node>::iterator end(){
        return adj_map.end();
    }

    uint getEraseMark(){
        erase_mark = nodes();
        while(adj_map.count(erase_mark))
            erase_mark+=1031; // prime to get out of clusters quickly 
        return erase_mark;
    }

    void eraseMarked(){ 
        if(erase_mark == 0) return;
        for(auto& pair : adj_map){
            for(auto index=0; index < pair.second.size(); index++){
                if(pair.second[index] == erase_mark)
                    quickRemoveEdge(pair.second, index--);
            }
        }
        for(auto pair = adj_map.begin(); pair != adj_map.end();){
            if(pair->second.size() == 0)
                pair = adj_map.erase(pair); // node in a tail, remov
            else
                ++pair;
        }
    }
};

#endif

