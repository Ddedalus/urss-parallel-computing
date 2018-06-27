/**
 * @brief Currently available Breadth First Search and Sum of Shortest Paths algorithms.
 * 
 * @file algorithms.h
 * @author Hubert Beres
 * @date 2018-06-27
 */
#ifndef __ALGO_H__
#define __ALGO_H__

#include <vector>
#include <deque>
#include <fstream>
#include <iostream>
#include <stdint.h>

#include "Bitset.h"

/**
 * @brief SSP building consecutive level sets for all vertices simultaneously by memoised iterative deepening.
 * 
 * Implemented using two Bitset instances per node. Behavious undefined if graph is not connected.
 * Space complexity: \$ O(n^2) \$
 * Time complexity:  \$ O(d \cross e \cross \frac{n}{w}) \$
 *  assuming a single machine instruction can logically-or two w-bit words in one go
 *  and d is the network diameter.
 * 
 * @param neighbours adjacency list representation of a directed, unweighted graph.
 * @param numEdges total no. edges in neighbours
 * @return uint64_t sum of lengths of shortest paths between every pair of vertices.  
 */
uint64_t sspBitset(const std::vector< std::vector<uint32_t> > &neighbours, uint32_t numEdges);

/**
 * @brief SSP by classical, queue based BFS applied to each node in turn.
 * Space complexity: \$ O(v) \$ worst case. 
 * Time complexity: \$ O(n^2 + n \cross e) \$
 * assuming constant queue and dequeue implementation
 * @param neighbours adjacency list representation of a directed, unweighted graph.
 * @param numEdges total no. edges in neighbours
 * @return uint64_t sum of lengths of shortest paths between every pair of vertices
 */
uint64_t sspBFS(const std::vector< std::vector<uint32_t> > &neighbours, uint32_t numEdges);

/**
 * @brief SSP for a given source node, computed by a queue based BFS.
 * Time complexity: \$ O(n + e) \$
 * Undefined if graph is not connected.
 * 
 * @param neighbours adjacency list representation of a directed, unweighted graph.
 * @param v the source node
 * @param numNodes in neighbours
 * @return uint64_t sum of lengths of shortest paths from v to each other node in the graph 
 */
uint64_t vertexBFS(const std::vector< std::vector<uint32_t> > &neighbours, uint32_t v, uint32_t numNodes);
#endif