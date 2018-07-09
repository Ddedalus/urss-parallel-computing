#include "public.h"
#include "tests.h"

#include <unordered_map>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]){
    uint32_t e = 0;
    auto g = readGraph("../data/g80.edges", e, false);
    cout <<g.size();
    return 0;
}