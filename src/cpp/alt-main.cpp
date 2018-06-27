#include "Bitset.h"
#include "graphIO.h"
// #include "tests.h"
#include "algorithms.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[]){

    // tests();

    // string fb("../data/web-cleaned.txt");
    string g("../data/g20.edges");  // remember to run from release build to get optimal performance

    uint32_t n, e;
    vector< vector<uint32_t> > neighbours = readGraph(g, n, e);

    cout << "BFS:" << sspBFS(neighbours, e) << endl;

    return 0;
}