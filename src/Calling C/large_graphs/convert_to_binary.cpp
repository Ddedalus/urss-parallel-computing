#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdint.h>

using namespace std;
// gain is of the order of 60/75 which is not worth the effort. Discarding
int main(int argc, char** argv){
    if(argc != 2){
        cout<<"Please specify a file as an anexecution argument"<<endl;
        exit(EXIT_FAILURE);
    }
    string filename(argv[1]);
    string outfilename = filename.substr(0, filename.find_last_of('.')) + ".bin";
    ifstream input(filename);
    ofstream output;
    output.open(outfilename, ios::binary | ios::out);

    string word;
    unsigned long numEdges = 0, numNodes = 0, directed =1;
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
    cout<<"Found: Nodes="<<numNodes<<" Edges="<<numEdges<<endl;

    if(!numEdges)
        cout<<"Couldn't find out how many edges are there!"<<endl;

    uint32_t from, to, counter = 0, maxNode = 0;
    while(input >> from >> to){
        output << from << to;
        counter++;
        if(maxNode < from || maxNode < to)
            maxNode = from < to ? to : from;
    }
    if(numEdges != 0 && numEdges != counter)
        cout<<"Number of edges different than declared!"<<endl;
    else if(numNodes == 0)
        numNodes = counter;

    if(maxNode >= numNodes)
        cout<<"Declared less nodes than the maximal ID found!"<<endl;
    if(numNodes == 0)
        numNodes = maxNode;

    output.clear();
    output.seekp(0, ios::beg);
    output << directed << numNodes << numEdges;  //write metadata at the beginning of the file

    output.close();
    input.close();

    return 0;
}