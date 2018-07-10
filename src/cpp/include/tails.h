#ifndef __TAILS_H__
#define __TAILS_H__

#include "public.h"
#include "algorithms.h"
#include "Bitset.h"

#include <deque>
#include <algorithm>


std::unordered_map<nodeId, std::vector<uint32_t>> cutTails(graph& g);
uint64_t sspBFStails(const graph& g);

#endif