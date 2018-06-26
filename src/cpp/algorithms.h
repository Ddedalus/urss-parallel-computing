#ifndef __ALGO_H__
#define __ALGO_H__

#include <vector>
#include <deque>
#include <fstream>
#include <iostream>
#include <stdint.h>

#include "Bitset.h"

uint64_t sspBitset(const std::vector< std::vector<uint32_t> > &neighbours, uint32_t numEdges);

uint64_t sspBFS(const std::vector< std::vector<uint32_t> > &neighbours, uint32_t numEdges);

// sum of the shortest paths from one vertex
uint64_t vertexBFS(const std::vector< std::vector<uint32_t> > &neighbours, uint32_t v, uint32_t numNodes);
#endif