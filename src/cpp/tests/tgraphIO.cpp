#include "tests.h"

using namespace std;

void graph_io_test(){

    cout << "\n     --  Graph IO  --   " << std::endl;

    string sample("../data/sample.graph.txt");
    uint32_t e;
    graph g = readGraph(sample, e);
    vector<uint32_t> v2 = {0,69,6,78,36,6,23,35};
    cout << "Read graph: "
    << (g.size() == 15) <<" "
    << (e == 19) << " "
    << (g[2].neigh == v2) << endl;
}