#ifndef __GRAPH_CLASS_H__
#define __GRAPH_CLASS_H__

#include "public.h"


using nodeID = uint;
using Node = std::vector<nodeID>;
// if needed, vector property table may be added later.
class Graph
{
    friend std::ostream &operator<<(std::ostream &os, Graph &g);
protected:
    uint no_edges;
    /* data */
public:
    uint edges() const {return no_edges;}
    virtual uint nodes() const = 0;
    virtual std::vector<nodeID>& operator[](nodeID pos) = 0;    // just get adjacency pair
    virtual void addEdge(nodeID from, nodeID to, bool directed)=0;
};

class mapGraph;
class vecGraph : public Graph{
private:
    std::vector<Node> adj_list;
public:
    vecGraph(uint size) : Graph(), adj_list(size){
        no_edges = 0;
    }
    vecGraph(mapGraph& g, std::unordered_map<nodeID, nodeID>& transform);
    vecGraph(mapGraph& g);
    uint nodes() const override {   return adj_list.size();  }
    std::vector<nodeID>& operator[](nodeID pos) override {
        return adj_list[pos];
    }
    void addEdge(nodeID from, nodeID to, bool directed = false) override;

    std::vector<Node>& data(){  return adj_list;    }

    // range based for loop support
    std::vector<Node>::iterator begin(){
        return adj_list.begin();
    }
    std::vector<Node>::iterator end(){
        return adj_list.end();
    }

    mapGraph& sparselyNumberedCopy();
};

class mapGraph : public Graph {
friend vecGraph;
private:
    std::unordered_map<nodeID, Node> adj_map;
    uint erase_mark;    // easy bulk removals enabled if this is not 0

    nodeID quickRemoveEdge(Node& neigh, uint which);
public:
    mapGraph() : Graph(), erase_mark(0){}
    mapGraph(vecGraph& g);
    uint nodes() const override {   return adj_map.size();}
    std::vector<nodeID>& operator[](nodeID pos) override {
        return adj_map[pos];
    }
    void addEdge(nodeID from, nodeID to, bool directed = false) override;
    
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

    void eraseMarked();

    vecGraph& toDense();
};

#endif

