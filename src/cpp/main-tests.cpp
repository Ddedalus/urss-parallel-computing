// #include "public.h"
// #include "tests.h"

#include <unordered_map>
#include <iostream>
#include <vector>

using namespace std;

unordered_map<uint32_t , vector<uint32_t>> getFancyGraph(){

    std::unordered_map<uint32_t , vector<uint32_t>> m;

    for(uint32_t i = 0; i < 110000; i++){
        m[2*i].push_back(i);
    }
    m.reserve(m.size());
    return m;
}

int main(int argc, char* argv[]){
    auto g = getFancyGraph();
    uint32_t e = 0;
    // auto g = readGraph("../data/sample.graph.txt", e, false);
    cout << "ugh" <<g.size();
    return 0;
}