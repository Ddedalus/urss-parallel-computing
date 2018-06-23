#include <iostream>
#include <vector>
#include <fstream>

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

	// Create mapping from node id to number of neighbours
	fp>>numNodes>>numEdges;
	neighbours.resize(numNodes);
	
	uint32_t from, to;
	for(uint32_t i = 0; i < numEdges; i++){
		fp >> from >> to;
		// version suitable for Higher computing
		neighbours[min(from, to)].push_back(max(from, to));
	}
	fp.close();
	return;
}

uint32_t vertexSSP(uint32_t v){
	uint32_t sum = 0;
	vector<bool> visited(numNodes, false);
	vector<uint32_t> fringe(numNodes);
	vector<uint32_t> newFringe(numNodes);
	fringe[0] = v;
	uint32_t d = 1;	
	while(! fringe.empty()){
		for(uint32_t x : fringe){
			for(auto n : neighbours[x]){
				if(! visited[n]){
					newFringe.push_back(n);
					visited[n] = true;
					sum+=d;
				}
			}
		}
		fringe.clear();
		fringe.swap(newFringe);
		d++;
	}	
	return sum;
}

// sum of shortest paths only to the vertices higher than this one
double vertexHigherSSP(uint32_t v){
	uint32_t sum = 0,d = 1;
	vector<bool> visited(numNodes, false);
	vector<uint32_t> fringe(numNodes - v);
	vector<uint32_t> newFringe(numNodes - v);
	fringe[0] = v;
	while(! fringe.empty()){
		for(uint32_t x : fringe){
			for(auto n : neighbours[x]){
				if(n > v && !visited[n-v-1]){
					newFringe.push_back(n);
					visited[n-v-1] = true;
					sum+=d;
				}
			}
		}
		fringe.clear();
		fringe.swap(newFringe);
		d++;
	}	
	return sum;
}

double serialASP(){
	double avg = 0.;
	for(uint32_t v = 0; v < numNodes; v++){
		avg += vertexSSP(v)* 1. / (numNodes - 1);
	}
	return avg /(1.0 * numNodes);

}

double serialHigherASP(){
	uint64_t avg = 0.;
	for(uint32_t v = 0; v < numNodes; v++){
		avg += vertexHigherSSP(v);
	}
	return avg * 2.0 /(numNodes * (numNodes - 1));

}


int main(){
	string path = "/home/hubert/Code/Warwick/BSP/external/asp/Networks/n5.edges";
	readGraph(path);
	for(uint32_t v = 0; v < numNodes; v++){
		// cout<<vertexSSP(v)<<endl;
	}
	// cout<< serialASP()<<endl;
	return 0;
}

