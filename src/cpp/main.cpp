#include "Bitset.h"
#include "graphIO.h"
#include "tests/tests.h"
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

    // cout << "Bitset:" << sspBitset(neighbours, e) << endl;
    // cout << "BFS:" << sspBFS(neighbours, e) << endl;
    cout << "Parallel Bitset:" << sspBitset(neighbours, e) << endl;

    return 0;
}

// int main(int argc, char**argv)
// {
//   if (argc != 2) {
//     printf("Specify edges file\n");
//     exit(EXIT_FAILURE);
//   }

//   uint32_t numEdges, numNodes;   
//   auto neighbours = readGraph(argv[1], numEdges, numNodes);

//   struct timeval start, finish, diff;

//   gettimeofday(&start, NULL);
//   uint64_t sum = sspBitset(neighbours, numEdges);
//   gettimeofday(&finish, NULL);

//   cout<< "Sum of shortest paths = " << sum << endl;
//   cout << "Average path length: " << sum * 1.0 /(numNodes * (numNodes - 1)) << endl;
 
//   timersub(&finish, &start, &diff);
//   double duration = (double) diff.tv_sec + (double) diff.tv_usec / 1000000.0;
//   cout << "Time = " << duration << endl;

//   return 0;
// }

// int main(int argc, char** argv){
//     if(argc < 2){
//         cout<<"Please specify a file as an anexecution argument"<<endl;
//         exit(EXIT_FAILURE);
//     }
//     string filename(argv[1]), outfilename;
//     if(argc < 3)
//         outfilename = filename.substr(0, filename.find_last_of('.')) + ".bin";
//     else
//         outfilename = argv[2];

//     convertToBinary(filename, outfilename);
//     uint32_t n, e;
//     readBinaryGraph(outfilename, n, e);
//     cout<<"Nodes: "<< n << " Edges" << e << endl;
//     return 0;

// }