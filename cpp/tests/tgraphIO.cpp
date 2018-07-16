#include "tests.h"

using namespace std;

void graph_io_test()
{

    cout << "\n     --  Graph IO  --   " << std::endl;

    string sample("../data/sample.graph.txt");
    vecGraph g(10);
    readGraph(g, sample);
    vector<uint32_t> v2 = {0, 78, 36, 6, 23, 35};
    cout << "Read graph (vec): "
         << (g.nodes() == 79) << " "    //the highest vertex label
         << (g.edges() == 17) << " "
         << (g[2] == v2) << endl;

    mapGraph mg;
    readGraph(mg, sample);
    cout << "Read graph (map): "
         << (mg.nodes() == 15) << " "   // is sparse
         << (mg.edges() == 17) << " "
         << (mg[2] == v2) << endl;
    cout<<endl;
}