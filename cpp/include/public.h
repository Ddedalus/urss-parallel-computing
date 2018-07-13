#ifndef __MAIN_ASP_H__
#define __MAIN_ASP_H__

#include <vector>
#include <unordered_map>
#include <omp.h>
#include <stdint.h>
#include <iostream>
#include <string>

#include "printing.h"

using nodeId = uint32_t;

class node{
public: 
    std::vector<nodeId> neigh;
    int pos;
    // constant time removal
    nodeId removeEdge(int whichEdge){
        auto temp = neigh[whichEdge];
        neigh[whichEdge] = neigh.back();
        neigh.pop_back();
        return temp;
    }
    nodeId& operator[](int pos){
        return neigh[pos];
    }
    int size(){
        return neigh.size();
    }
};


using graph = std::unordered_map<nodeId, node>;

#endif