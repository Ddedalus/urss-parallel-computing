#ifndef __TAILS_H__
#define __TAILS_H__

#include "algorithms.h"
#include "Bitset.h"

#include "iostream"
#include "vector"
#include "list"
#include "deque"
#include "unordered_map"

std::unordered_map<uint32_t, std::vector<uint32_t>> cutTails(graph& g);
uint64_t sspBFStails(graph g);

#endif