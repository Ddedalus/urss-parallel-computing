#include <vector>
#include <fstream>
#include <iostream>

#include <stdint.h>
#include <assert.h>
#include <sys/time.h>

#include "Bitset.h"

using namespace std;

vector< vector<uint32_t> > readGraph(string filename, uint32_t &numEdges, uint32_t &numNodes)
{
	ifstream fp;
	fp.open(filename, ios::in);

	// Read edge list from file
	if (!fp) {
		cout << "Can't open " << filename << endl;
		exit(EXIT_FAILURE);
	}

	fp >> numNodes >> numEdges;
	vector< vector<uint32_t> > neighbours(numNodes);
	
	uint32_t from, to;
	for(uint32_t i = 0; i < numEdges; i++){
		fp >> from >> to;
		neighbours[from].push_back(to);
		neighbours[to].push_back(from);
	}
	fp.close();
	return neighbours;
}

// Compute sum of all shortest paths
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
      if(! reaching[i].is_full())
        done = false;
    }
    
    dist++;
  }

  return sum;
}

int main(int argc, char**argv)
{
  if (argc != 2) {
    printf("Specify edges file\n");
    exit(EXIT_FAILURE);
  }

  uint32_t numEdges, numNodes;   
  auto neighbours = readGraph(argv[1], numEdges, numNodes);

  struct timeval start, finish, diff;

  gettimeofday(&start, NULL);
  uint64_t sum = sspBitset(neighbours, numEdges);
  gettimeofday(&finish, NULL);

  cout<< "Sum of shortest paths = " << sum << endl;
  cout << "Average path length: " << sum * 1.0 /(numNodes * (numNodes - 1)) << endl;
 
  timersub(&finish, &start, &diff);
  double duration = (double) diff.tv_sec + (double) diff.tv_usec / 1000000.0;
  cout << "Time = " << duration << endl;

  return 0;
}
