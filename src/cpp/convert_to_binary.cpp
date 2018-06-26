#include <iostream>
#include <string>

#include "graphIO.h"

using namespace std;
// gain is of the order of 60/75 which is not worth the effort. Discarding
int main(int argc, char** argv){
    if(argc < 2){
        cout<<"Please specify a file as an anexecution argument"<<endl;
        exit(EXIT_FAILURE);
    }
    string filename(argv[1]), outfilename;
    if(argc < 3)
        outfilename = filename.substr(0, filename.find_last_of('.')) + ".bin";
    else
        outfilename = argv[2];

    convertToBinary(filename, outfilename);
    uint32_t n, e;
    readBinaryGraph(outfilename, n, e);
    cout<<"Nodes: "<< n << " Edges" << e << endl;
    return 0;

}