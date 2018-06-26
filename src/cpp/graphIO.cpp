#include "graphIO.h"

using namespace std;

void convertToBinary(std::string filename, std::string outfilename){
    std::ifstream input(filename);
    std::ofstream output(outfilename, std::ios::binary | std::ios::out);

    std::string word;
    uint32_t numEdges = 0, numNodes = 0, directed =1;
    while(input.peek() == '#'){ // comments section
        std::string line; std::getline(input, line);
        std::istringstream ss(line);
        while(ss>>word){
            if(word.find("Edges") != std::string::npos)
                ss >> numEdges;
            else if(word.find("Vertices") != std::string::npos ||
                    word.find("Nodes") != std::string::npos)
                ss >> numNodes;
            else if(word.find("Undirected") != std::string::npos)
                directed = 0;
        }
    }
    std::cout<<"Found: Nodes="<<numNodes<<" Edges="<<numEdges<< std::endl;

    if(!numEdges)
        std::cout<<"Couldn't find out how many edges are there!" << std::endl;

    uint32_t from, to, counter = 0, maxNode = 0;
    while(input >> from >> to){
        output.write((char*) &from, sizeof(from));
        output.write((char *) &to, sizeof(to));
        counter++;
        if(maxNode < from || maxNode < to)
            maxNode = from < to ? to : from;
    }
    if(numEdges != 0 && numEdges != counter)
        std::cout<<"Number of edges different than declared!" << std::endl;
    else if(numNodes == 0)
        numNodes = counter;

    if(maxNode >= numNodes)
        std::cout<<"Declared less nodes than the maximal ID found!"<< std::endl;
    if(numNodes == 0)
        numNodes = maxNode;

    output.clear();
    output.seekp(0, std::ios::beg);
    for(auto i : {directed, numNodes, numEdges})
        output.write((char*) &i, sizeof(i));  //write metadata at the beginning of the file

    output.close();
    input.close();
}

vector< vector<uint32_t> > readBinaryGraph(string filename, uint32_t &numEdges, uint32_t &numNodes)
{
	ifstream fp;
	fp.open(filename, ios::in | ios::binary);

	// Read edge list from file
	if (!fp) {
		cout << "Can't open " << filename << endl;
		exit(EXIT_FAILURE);
	}
    uint32_t directed;
    
    fp.read((char*) &directed, sizeof(directed));
    fp.read((char*) &numNodes, sizeof(numNodes));
    fp.read((char*) &numEdges, sizeof(numEdges));

    
	vector< vector<uint32_t> > neighbours(numNodes);
	
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

