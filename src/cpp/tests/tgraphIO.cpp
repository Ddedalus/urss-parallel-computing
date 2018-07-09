#include "tests.h"

using namespace std;

void graph_io_test(){

    cout << "\n     --  Graph IO  --   " << std::endl;

    string sample("../data/sample.graph.txt");
    string binary("../output/sample.bin");
    uint32_t e;
    graph g = readGraph(sample, e);
    cout << "Graph created" << endl;
    // cout<<"Size: " << g.size() << endl;
    // for(auto li : g){
    //     cout<<li.first<<": ";
    //     for(auto el : li.second){
    //         cout<< el << " ";
    //     }
    //     cout << endl;
    // }
    // cout<<"Fuck";
    // g.clear();
    // convertToBinary(sample, binary);
    // auto neighbours = readBinaryGraph(binary, n ,e);
    // auto neigh = g.at(2);
    // for(auto  n : neigh)
    //     cout << n << " ";
    cout<< "Still alive";
}