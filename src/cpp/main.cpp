#include "Bitset.h"
#include "graphIO.h"
#include "tests/tests.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[]){

    // all_tests();
    string sample("./data/sample.graph.txt");
    string binary("./output/sample.bin");
    convertToBinary(sample, binary);

    uint32_t n, e;
    auto neighbours = readBinaryGraph(binary, n ,e);
    for(auto  n : neighbours[2])
        cout << n << " ";
    cout<< endl;
}