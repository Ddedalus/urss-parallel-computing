#include <iostream>
#include <vector>
#include <fstream>
#include <deque>

using namespace std;
// Number of nodes and edges
uint32_t numNodes;
uint32_t numEdges;
//  An adjacency list representation
vector< vector<uint32_t> > neighbours;

void readGraph(string filename)
{
	ifstream fp;
	fp.open(filename, ios::in);

	// Read edge list from file
	if (!fp) {
		cout << "Can't open " << filename;
		return;
	}

	fp >> numNodes >> numEdges;
	neighbours.resize(numNodes);
	
	uint32_t from, to;
	for(uint32_t i = 0; i < numEdges; i++){
		fp >> from >> to;
		neighbours[from].push_back(to);
		neighbours[to].push_back(from);
	}
	fp.close();
	return;
}

struct qNode{
	uint32_t node;
	uint32_t distance;
};

// sum of the shortest paths from one vertex
double vertexSSP(uint32_t v){
	uint32_t sum = 0;
	vector<bool> visited(numNodes, false);
	deque<qNode> q;	// greater memory flexibility
	q.push_back({v, 0}); visited[v] = true;
	while(!q.empty()){
		qNode current = q.front(); q.pop_front();
		for(uint32_t n : neighbours[current.node]){
			if(! visited[n]){
				q.push_back({n, current.distance + 1});
				sum += current.distance + 1;
				visited[n] = true;
			}
		}
	}	
	return sum;
}

double serialASP(){
	uint64_t avg = 0;
	for(uint32_t v = 0; v < numNodes; v++){
		avg += vertexSSP(v);
	}
	return avg * 1.0 /(numNodes * (numNodes - 1));

}

int main(){
	string path = "/home/hubert/Code/Warwick/BSP/external/asp/Networks/n5.edges";
	readGraph(path);
	cout<< serialASP()<<endl;
	return 0;
}

