#include "graphIO.h"

using namespace std;

void convertToBinary(string filename, string outfilename){
    ifstream input(filename);
    ofstream output(outfilename, ios::binary | ios::out);

    string word;
    uint32_t numEdges = 0, numNodes = 0, directed =1;
    while(input.peek() == '#'){ // comments section
        string line; getline(input, line);
        istringstream ss(line);
        while(ss>>word){
            if(word.find("Edges") != string::npos)
                ss >> numEdges;
            else if(word.find("Vertices") != string::npos ||
                    word.find("Nodes") != string::npos)
                ss >> numNodes;
            else if(word.find("Undirected") != string::npos)
                directed = 0;
        }
    }
    cout<< "Read graph: "<<"Found: Nodes="<<numNodes<<" Edges="<<numEdges<< endl;

    if(!numEdges)
        cout<< "Read graph: "<<"Couldn't find out how many edges are there!" << endl;

    uint32_t from, to, counter = 0, maxNode = 0;
    while(input >> from >> to){
        output.write((char*) &from, sizeof(from));
        output.write((char *) &to, sizeof(to));
        counter++;
        if(maxNode < from || maxNode < to)
            maxNode = from < to ? to : from;
    }
    if(numEdges != 0 && numEdges != counter)
        cout<< "Read graph: "<<"Number of edges different than declared!" << endl;
    else if(numNodes == 0)
        numNodes = counter;

    if(maxNode >= numNodes)
        cout<< "Read graph: "<<"Declared less nodes than the maximal ID found!"<< endl;
    if(numNodes == 0)
        numNodes = maxNode;

    output.clear();
    output.seekp(0, ios::beg);
    for(auto i : {directed, numNodes, numEdges})
        output.write((char*) &i, sizeof(i));  //write metadata at the beginning of the file

    output.close();
    input.close();
}

graph readGraph(string filename, uint32_t &numEdges, bool directed){
    ifstream input(filename);
    if(!input){
        cout<< "Read graph: "<<"Error opening " << filename << endl;
        exit(EXIT_FAILURE);
    }
    while(input.peek() == '#'){ // comments section
        string line; getline(input, line);
    }

    graph neighbours;
    numEdges=0;
    uint32_t from, to;
    while(input >> from >> to){
        neighbours[from].push_back(to);
    	if(!directed) neighbours[to].push_back(from);
        numEdges++;
    }
    cout<< "Read graph: Nodes="<<neighbours.size()
                <<" Edges=" << numEdges
                <<" Directed=" << (directed ? "true" : "false") << endl;
    input.close();
    neighbours.reserve(neighbours.size());
    return neighbours;
}

graph readBinaryGraph(string filename, uint32_t &numEdges, uint32_t &numNodes){
	ifstream fp;
	fp.open(filename, ios::in | ios::binary);

	// Read edge list from file
	if (!fp) {
		cout<< "Read graph: " << "Can't open " << filename << endl;
		exit(EXIT_FAILURE);
	}
    uint32_t directed;
    
    fp.read((char*) &directed, sizeof(directed));
    fp.read((char*) &numNodes, sizeof(numNodes));
    fp.read((char*) &numEdges, sizeof(numEdges));

    
	graph neighbours(numNodes);
	
	uint32_t from, to;
	for(uint32_t i = 0; i < numEdges; i++){
    	fp.read((char*) &from, sizeof(from));
    	fp.read((char*) &to, sizeof(to));

		neighbours[from].push_back(to);
    	if(directed==0)	neighbours[to].push_back(from);
	}
	fp.close();
	return neighbours;
}

graph readRawGraph(string filename, uint32_t &numEdges, uint32_t &numNodes){
	ifstream fp;
	fp.open(filename, ios::in);

	// Read edge list from file
	if (!fp) {
		cout<< "Read graph: " << "Can't open " << filename << endl;
		exit(EXIT_FAILURE);
	}

	fp >> numNodes >> numEdges;
	graph neighbours(numNodes);
	
	uint32_t from, to;
	for(uint32_t i = 0; i < numEdges; i++){
		fp >> from >> to;
		neighbours[from].push_back(to);
		neighbours[to].push_back(from);
	}
	fp.close();
	return neighbours;
}

