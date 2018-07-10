#include "algorithms.h"

using namespace std;

int assignPositions(graph &g){
    int count=0;
    for(auto& n : g)
        n.second.pos = count++;
    return count;
}

uint64_t vertexBFS(graph &g, node v){
	uint64_t sum = 0, count = 1;
	vector<bool> visited(g.size(), false);
	deque<qNode> q;	// greater memory flexibility
	q.push_back({v, 0}); visited[v.pos] = true;

	while(!q.empty() && count <= g.size()){
		qNode current = q.front(); q.pop_front();
		for(auto n : current.n.neigh){
      node ne = g[n];
			if(! visited[ne.pos]){
				q.push_back({ne, current.dist + 1});
				sum += current.dist + 1;
				visited[ne.pos] = true;
        count++;
			}
		}
	}
  if(count < g.size())
    cout<<"Haven't reached " << g.size() - count << " nodes" << endl;
	return sum;
}

uint64_t sspBFS(graph &g, uint32_t numEdges){
	uint64_t sum = 0;
  assignPositions(g);
	for(auto iter = g.begin(); iter != g.end(); iter++){
	    sum += vertexBFS(g, iter->second);
	}
	return sum;

}

uint64_t sspBitset(graph &g, uint32_t numEdges)
{
  // Sum of distances
  uint64_t sum = 0;
  assignPositions(g);
  // Initialise reaching vector for each node
  vector< Bitset > reaching(g.size(), Bitset(g.size()));
  vector< Bitset > reachingNext(g.size(), Bitset(g.size()));
  for (auto& pair : g) {
    auto i = pair.second.pos;
    reaching[i].set(i, true);
    reachingNext[i].set(i, true);
  }

  // Distance increases on each iteration
  uint32_t dist = 1;

  bool done = false;
  while (! done) {
    // For each node
    for (auto& pair : g) {
      // For each neighbour
      for (auto n : pair.second.neigh){
        reachingNext[g[n].pos] += reaching[g[n].pos];
      }
      // add the shortest paths found
      sum += dist * reachingNext[pair.second.pos].diff_size(reaching[pair.second.pos]);
    }

    // For each node, update reaching vector
    done = true;
    for (auto& pair : g) {
      reaching[pair.second.pos].copy_from(reachingNext[pair.second.pos]);
      if(! reaching[pair.second.pos].is_full()){
        done = false;
        // cout<<"Node "<< i <<" not full, size: "<< reaching[i].count()<<endl;
      }
    }
    dist++;
  }

  return sum;
}

// uint64_t sspParaBitset(graph &g, uint32_t numEdges)
// {
//   // Sum of distances
//   uint64_t sum = 0;
//   assignPositions(g);
//   // Initialise reaching vector for each node
//   vector< Bitset > reaching(g.size(), Bitset(g.size()));
//   vector< Bitset > reachingNext(g.size(), Bitset(g.size()));
//   for (auto& pair : g) {
//     reaching[pair.second.pos].set(i, true);
//     reachingNext[pair.second.pos].set(i, true);
//   }

//   // Distance increases on each iteration
//   uint32_t dist = 1;

//   bool done = false;
//   while (! done) {
//     #pragma omp parallel
//     {
//       // For each node
//       #pragma omp for reduction(+ : sum)
//       for (auto& pair : g) {
//         // For each neighbour
//         for (auto n : pair.second.neigh){
//           reachingNext[g[n].pos] += reaching[g[n.pos]];
//         }
//         // add the shortest paths found
//         sum += dist * reachingNext[pair.second.pos].diff_size(reaching[pair.second.pos]);
//       }

//       // For each node, update reaching vector
//       done = true;
//       #pragma omp for reduction(&& : done)
//       for (auto& pair : g) {
//         reaching[pair.second.pos].copy_from(reachingNext[pair.second.pos]);
//         if(! reaching[pair.second.pos].is_full()){
//           done = false;
//           // cout<<"Node "<< i <<" not full, size: "<< reaching[i].count()<<endl;
//         }
//       }
//     }
//     dist++;
//   }

//   return sum;
// }