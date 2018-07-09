#ifndef __MAIN_ASP_H__
#define __MAIN_ASP_H__

#include <vector>
#include <unordered_map>
#include <omp.h>
#include <stdint.h>
#include <iostream>
#include <string>


using nodeId = uint32_t;

class node{
public: 
    int pos;
    std::vector<nodeId> neigh;
    // constant time removal
    nodeId removeEdge(int whichEdge){
        auto temp = neigh[whichEdge];
        neigh[whichEdge] = neigh.back();
        neigh.pop_back();
        return temp;
    }
};


using graph = std::unordered_map<nodeId, node>;

#endif