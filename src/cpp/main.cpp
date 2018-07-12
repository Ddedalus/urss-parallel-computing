#include "public.h"

#include "Bitset.h"
#include "graphIO.h"
#include "algorithms.h"
#include "Timer.h"
#include "tails.h"
#include "Graph.h"

// list directory:
#include <sys/types.h>
#include <dirent.h>

using namespace std;

void read_directory(string inputPath, vector<string> &names)
{
    DIR *dirp = opendir(inputPath.c_str());
    struct dirent *dp;
    while ((dp = readdir(dirp)) != NULL)
    {
        if (dp->d_type == 8)
            names.push_back(dp->d_name);
    }
    closedir(dirp);
    std::sort(names.begin(), names.end());
}

int main(int argc, char *argv[])
{

    string inputPath("../data/barabasi/");

    vector<string> inputNames;
    read_directory(inputPath, inputNames);
    
    Timer t;
    for(auto p : inputNames){
        mapGraph mg;
        readGraph(mg, inputPath + p);
        t.start("Bitset: " + p);
        sspBitset(mg);
        t.print();
    }
    // mapGraph mg;

    // vecGraph vg(1000);
    // readGraph(mg, path);
    // readGraph(vg, path);
    // Timer t;

    // t.start("Tails BFS");
    // cout << sspBFStails(mg) << endl;
    // t.print();

    // t.start("Standard BFS");
    // cout << sspBFS(vg) << endl;
    // t.print();

    // t.start("Bitset on map");
    // cout << sspBitset(mg) << endl;
    // t.print();

    // t.start("Bitset on vec");
    // cout << sspBitset(vg) << endl;
    // t.print();

    return 0;
}

// Timer t("Parallel Bitset");
// auto check = sspParaBitset(neighbours, e);
// t.print();

// t.start("Bitset");
// check = sspBitset(neighbours, e);
// t.print();

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