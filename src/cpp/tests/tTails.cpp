#include "tests.h"

using namespace std;
void tails_test()
{
    cout << "\n     --    Tails    --   " << std::endl;
    mapGraph g;
    readGraph(g, "../data/sample.graph.txt");
    auto tails = cutTails(g);
    // cout << tails << endl;

    vector<nodeId> v2 = {1, 2, 1}, v42 = {1, 1, 1, 1, 1};
    cout << "cutTails: " << (g.nodes() == 6) << " "
         << (tails[42] == v42) << " "
         << (tails[2] == v2) << endl;

    mapGraph g2;
    readGraph(g2, "../data/sample.graph.txt");
    cout << "BFS with tails: " << (sspBFStails(g2) == 536) << endl;
}