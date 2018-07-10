#include "tests.h"

using namespace std;
void tails_test(){
    cout << "\n     --    Tails    --   " << std::endl;
    uint32_t e;
    auto g = readGraph("../data/sample.graph.txt", e);
    auto tails = cutTails(g);

    vector<nodeId> v2 = {1, 2, 1}, v42 = {1, 1, 1, 1, 1};
    cout << "cutTails: " << (g.size() == 6) << " "
         << (tails[42] == v42) << " "
         << (tails[2] == v2);

}