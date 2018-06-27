#include "algorithms.h"

using namespace std;

struct qNode{
	uint32_t node;
	uint32_t distance;
};

uint64_t vertexBFS(const vector< vector<uint32_t> > &neighbours, uint32_t v, uint32_t numNodes){
	uint64_t sum = 0, count = 1;
	vector<bool> visited(numNodes, false);
	deque<qNode> q;	// greater memory flexibility
	q.push_back({v, 0}); visited[v] = true;
	while(!q.empty() && count < numNodes){
		qNode current = q.front(); q.pop_front();
		for(uint32_t n : neighbours[current.node]){
			if(! visited[n]){
				q.push_back({n, current.distance + 1});
				sum += current.distance + 1;
				visited[n] = true;
        count++;
			}
		}
	}
  if(count < numNodes)
    cout<<"Haven't reached " << numNodes - count << " nodes" << endl;
	return sum;
}

uint64_t sspBFS(const vector< vector<uint32_t> > &neighbours, uint32_t numEdges){
      uint32_t numNodes = neighbours.size();
	uint64_t sum = 0;
	for(uint32_t v = 0; v < numNodes; v++){
	    sum += vertexBFS(neighbours, v, numNodes);
	}
	return sum;

}

uint64_t sspBitset(const vector< vector<uint32_t> > &neighbours, uint32_t numEdges)
{
  uint32_t numNodes = neighbours.size();
  // Sum of distances
  uint64_t sum = 0;

  // Initialise reaching vector for each node
  vector< Bitset > reaching(numNodes, Bitset(numNodes));
  vector< Bitset > reachingNext(numNodes, Bitset(numNodes));
  for (uint64_t i = 0; i < numNodes; i++) {
    reaching[i].set(i, true);
    reachingNext[i].set(i, true);
  }

  // Distance increases on each iteration
  uint32_t dist = 1;

  bool done = false;
  while (! done) {
    // For each node
    for (int i = 0; i < numNodes; i++) {
      // For each neighbour
      for (auto n : neighbours[i]){
        reachingNext[i] += reaching[n];
      }

      // add the shortest paths found
      sum += dist * reachingNext[i].diff_size(reaching[i]);
    }

    // For each node, update reaching vector
    done = true;
    for (int i = 0; i < numNodes; i++) {
      reaching[i].copy_from(reachingNext[i]);
      if(! reaching[i].is_full()){
        done = false;
        // cout<<"Node "<< i <<" not full, size: "<< reaching[i].count()<<endl;
      }
    }
    
    dist++;
  }

  return sum;
}