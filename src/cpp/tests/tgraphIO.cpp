#include "tests.h"
#include <string>
#include <iostream>

using namespace std;

void graph_io_test(){

    cout << "\n     --  Graph IO  --   " << std::endl;

    string sample("../data/sample.graph.txt");
    string binary("../output/sample.bin");
    convertToBinary(sample, binary);

    uint32_t n, e;
    auto neighbours = readBinaryGraph(binary, n ,e);
    for(auto  n : neighbours[2])
        cout << n << " ";
    cout<< "\n Read, Nodes=" << n << " Edges=" << e << endl;
}