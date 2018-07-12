#ifndef __TAILS_H__
#define __TAILS_H__

#include "public.h"
#include "algorithms.h"
#include "Bitset.h"

#include <deque>
#include <algorithm>

using tailsMap = std::unordered_map<nodeId, std::vector<uint>>;

uint64_t sspBFStails(const mapGraph &g);
tailsMap transfromTails(tailsMap &tails, transformMap &tr);
inline uint tailContribution(uint t, uint n, uint64_t ssp);
uint64_t vertexTailsBFS(vecGraph &g, tailsMap tails, nodeID vid);
tailsMap cutTails(mapGraph &g);


#endif