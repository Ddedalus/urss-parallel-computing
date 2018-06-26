#ifndef __READ_GRAPH_H__
#define __READ_GRAPH_H__

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdint.h>
#include <vector>

void convertToBinary(std::string filename, std::string outfilename);

std::vector< std::vector<uint32_t> > readBinaryGraph(std::string filename, uint32_t &numEdges, uint32_t &numNodes);

std::vector< std::vector<uint32_t> > readGraph(std::string filename, uint32_t &numEdges, uint32_t &numNodes);

#endif