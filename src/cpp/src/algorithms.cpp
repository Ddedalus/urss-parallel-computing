#include "algorithms.h"

using namespace std;

struct qNode{
	uint32_t node;
	uint32_t distance;
};

uint64_t vertexBFS(const graph &g, uint32_t v){
	uint64_t sum = 0, count = 1;
	vector<bool> visited(g.size(), false);
	deque<qNode> q;	// greater memory flexibility
	q.push_back({v, 0}); visited[v] = true;
	while(!q.empty() && count < g.size()){
		qNode current = q.front(); q.pop_front();
		for(node n : g.at(current.node)){
			if(! visited[n]){
				q.push_back({n, current.distance + 1});
				sum += current.distance + 1;
				visited[n] = true;
        count++;
			}
		}
	}
  if(count < g.size())
    cout<<"Haven't reached " << g.size() - count << " nodes" << endl;
	return sum;
}

uint64_t sspBFS(const graph &g, uint32_t numEdges){
	uint64_t sum = 0;
	for(auto iter = g.begin(); iter != g.end(); iter++){
	    sum += vertexBFS(g, iter->first);
	}
	return sum;

}

uint64_t sspBitset(const graph &g, uint32_t numEdges)
{
  // Sum of distances
  uint64_t sum = 0;

  // Initialise reaching vector for each node
  vector< Bitset > reaching(g.size(), Bitset(g.size()));
  vector< Bitset > reachingNext(g.size(), Bitset(g.size()));
  for (uint64_t i = 0; i < g.size(); i++) {
    reaching[i].set(i, true);
    reachingNext[i].set(i, true);
  }

  // Distance increases on each iteration
  uint32_t dist = 1;

  bool done = false;
  while (! done) {
    // For each node
    for (int i = 0; i < g.size(); i++) {
      // For each neighbour
      for (auto n : g.at(i)){
        reachingNext[i] += reaching[n];
      }

      // add the shortest paths found
      sum += dist * reachingNext[i].diff_size(reaching[i]);
    }

    // For each node, update reaching vector
    done = true;
    for (int i = 0; i < g.size(); i++) {
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

uint64_t sspParaBitset(const graph &g, uint32_t numEdges)
{
  // Sum of distances
  uint64_t sum = 0;

  // Initialise reaching vector for each node
  vector< Bitset > reaching(g.size(), Bitset(g.size()));
  vector< Bitset > reachingNext(g.size(), Bitset(g.size()));
  for (uint64_t i = 0; i < g.size(); i++) {
    reaching[i].set(i, true);
    reachingNext[i].set(i, true);
  }

  // Distance increases on each iteration
  uint32_t dist = 1;
  cout<<"Parallel with " << omp_get_num_procs() << "procs available\n";

  bool done = false;
  while (! done) {
    // For each node
    #pragma omp parallel
    {
      uint64_t l_sum = 0;
      #pragma omp for reduction(+ : sum)
      for (int i = 0; i < g.size(); i++) {
        for (auto n : g.at(i)){
          reachingNext[i] += reaching[n];
        }

        // add the shortest paths found
        sum += dist * reachingNext[i].diff_size(reaching[i]);
      }
      
      // For each node, update reaching vector
      done = true;
      #pragma omp for
      for (int i = 0; i < g.size(); i++) {
        reaching[i].copy_from(reachingNext[i]);
        if(! reaching[i].is_full()){
          done = false;
        }
      }
    }
      dist++;
  }

  return sum;
}