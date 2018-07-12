#include "public.h"

#include "Bitset.h"
#include "graphIO.h"
#include "algorithms.h"
#include "Timer.h"
#include "tails.h"
#include "Graph.h"

#include "RecordWriter.h"
#include "RunRecord.h"

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

    string outPath("../output/o1.csv");
    string inputPath("../data/barabasi/");

    cout<<"Record writer counter: "<< RecordWriter::run_counter_path <<endl;
    RecordWriter rw(outPath);
    vector<string> inputNames;
    read_directory(inputPath, inputNames);
    Timer t;
    map<string, string> c = {
        {"algorithm", "bitset"},
        {"threads", "1"},
        {"machine", "dell"},
        {"graphRepresentation", "map"},
        {"graphType", "barabasi"}};
    cout<<"c="<<c<<endl;
    for (auto p : inputNames)
    {
        RunRecord r(c);
        mapGraph mg;
        readGraph(mg, inputPath + p);
        t.start("Bitset: " + p);
        sspBitset(mg);
        r["runtime"] = to_string(t.getElapsed());
        t.print();
        r["instance"] = p;
        rw.write(r);
    }

    return 0;
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

// Timer t("Parallel Bitset");
// auto check = sspParaBitset(neighbours, e);
// t.print();

// t.start("Bitset");
// check = sspBitset(neighbours, e);
// t.print();
