#ifndef __READ_GRAPH_H__
#define __READ_GRAPH_H__

#include "public.h"

#include "Graph.h"

#include <fstream>
#include <sstream>
#include <string>

// void convertToBinary(std::string filename, std::string outfilename);

// graph readBinaryGraph(std::string filename, uint32_t &numEdges, uint32_t &numNodes);

void readGraph(Graph& neighbours, std::string filename, bool directed=false);

#endif