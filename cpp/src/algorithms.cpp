#include "algorithms.h"

using namespace std;

uint64_t vertexBFS(vecGraph& g, nodeID v_id){
	uint64_t sum = 0, count = 1;
	vector<bool> visited(g.nodes(), false);
	deque<qNodeLight> q;	// greater memory flexibility
	q.push_back({v_id, 0}); visited[v_id] = true;

	while(!q.empty() && count < g.nodes()){
		qNodeLight current = q.front(); q.pop_front();
		for(nodeId n : g[current.n]){
			if(! visited[n]){
				q.push_back({n, current.dist + 1});
				sum += current.dist + 1;
				visited[n] = true;
        count++;
			}
		}
	}
  if(count < g.nodes())
    cout<<"Haven't reached " << g.nodes() - count << " nodes" << endl;
	return sum;
}

uint64_t sspBFS(vecGraph &g){
	uint64_t sum = 0;
	for(uint i = 0; i < g.nodes(); i++){
	    sum += vertexBFS(g, i);
	}
	return sum;
}

uint64_t sspBFS(mapGraph& g){
  auto dg = vecGraph(g);
  return sspBFS(dg);
}

uint64_t sspBitset(vecGraph &g)
{
  // Sum of distances
  uint64_t sum = 0;
  // Initialise reaching vector for each node
  vector< Bitset > reaching(g.nodes(), Bitset(g.nodes()));
  vector< Bitset > reachingNext(g.nodes(), Bitset(g.nodes()));
  for (uint i = 0; i < g.nodes(); i++) {
    reaching[i].set(i, true);
    reachingNext[i].set(i, true);
  }

  // Distance increases on each iteration
  uint32_t dist = 1;

  bool done = false;
  while (! done) {
    // For each node
    for (uint i = 0; i < g.nodes(); i++) {
      // For each neighbour
      for (auto n : g[i]){
        reachingNext[i] += reaching[n];
      }
      // add the shortest paths found
      sum += dist * reachingNext[i].diff_size(reaching[i]);
    }
    // For each node, update reaching vector
    done = true;
    for (uint i = 0; i < g.nodes(); i++) {
      reaching[i].copy_from(reachingNext[i]);
      if(done && !reaching[i].is_full()){
        done = false;
        // cout<<"Node "<< i <<" not full, size: "<< reaching[i].count()<<endl;
      }
    }
    dist++;
  }

  return sum;
}

uint64_t sspBitset(mapGraph &g){
  auto dg = vecGraph(g);
  return sspBitset(dg);
}