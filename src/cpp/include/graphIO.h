#ifndef __READ_GRAPH_H__
#define __READ_GRAPH_H__

#include "public.h"

#include <fstream>
#include <sstream>
#include <string>

void convertToBinary(std::string filename, std::string outfilename);

graph readBinaryGraph(std::string filename, uint32_t &numEdges, uint32_t &numNodes);

graph readGraph(std::string filename, uint32_t &numEdges, bool directed=false);

#endif