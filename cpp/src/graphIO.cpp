#include "graphIO.h"

using namespace std;

void readGraph(Graph& neighbours, string filename, bool directed){
    ifstream input(filename);
    if(!input){
        cout<< "Read graph: "<<"Error opening " << filename << endl;
        exit(EXIT_FAILURE);
    }
    while(input.peek() == '#'){ // comments section
        string line; getline(input, line);
    }

    uint32_t from, to;
    while(input >> from >> to)
		neighbours.addEdge(from, to, directed);

    input.close();
}

// graph readBinaryGraph(string filename, uint32_t &numEdges, uint32_t &numNodes){
// 	ifstream fp;
// 	fp.open(filename, ios::in | ios::binary);

// 	// Read edge list from file
// 	if (!fp) {
// 		cout<< "Read graph: " << "Can't open " << filename << endl;
// 		exit(EXIT_FAILURE);
// 	}
//     uint32_t directed;
    
//     fp.read((char*) &directed, sizeof(directed));
//     fp.read((char*) &numNodes, sizeof(numNodes));
//     fp.read((char*) &numEdges, sizeof(numEdges));

    
// 	graph neighbours(numNodes);
	
// 	uint32_t from, to;
// 	for(uint32_t i = 0; i < numEdges; i++){
//     	fp.read((char*) &from, sizeof(from));
//     	fp.read((char*) &to, sizeof(to));

// 		neighbours[from].neigh.push_back(to);
//     	if(directed==0)	neighbours[to].neigh.push_back(from);
// 	}
// 	fp.close();
// 	return neighbours;
// }

// graph readRawGraph(string filename, uint32_t &numEdges, uint32_t &numNodes){
// 	ifstream fp;
// 	fp.open(filename, ios::in);

// 	// Read edge list from file
// 	if (!fp) {
// 		cout<< "Read graph: " << "Can't open " << filename << endl;
// 		exit(EXIT_FAILURE);
// 	}

// 	fp >> numNodes >> numEdges;
// 	graph neighbours(numNodes);
	
// 	uint32_t from, to;
// 	for(uint32_t i = 0; i < numEdges; i++){
// 		fp >> from >> to;
// 		neighbours[from].neigh.push_back(to);
// 		neighbours[to].neigh.push_back(from);
// 	}
// 	fp.close();
// 	return neighbours;
// }