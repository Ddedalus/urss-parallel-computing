#include "tails.h"

using namespace std;

unordered_map<uint32_t, std::vector<uint32_t>> cutTails(graph& g){
    unordered_map<uint32_t, vector<uint32_t>> tails;
    node null = g.size() + 1;
    auto count = 0;
    for(node i = 0; i < g.size(); i++){
        if(g[i].size() != 1) continue;

        int t = 1;  //length of the tail
        node current = g[i][0], previous=i;
        g[i][0] = null;
        while(g[current].size() == 2){
            t++;
            // select the other neighbour
            node next = g[current][0] == previous ? g[current][0] : g[current][1];
            g[current][0] = g[current][1] = null;   //remove edges from g
            previous = current; current = next;
        }
        count += t;
        // current is now the root:
        tails[current].push_back(t);
    }
    tails.reserve(tails.size());    // rehash to allow optimal capacity

    //clean up g:
    cout<<"Removed " << count << "vertices as tails" << endl;
    graph gp(g.size() - count);
    for(auto neigh : g){
        if(neigh.size() == 1) continue; // was certainly removed
        else if(neigh.size() == 2 && neigh[0]==null)
            continue; // was removed
        else{
            vector<node> new_adj;
            for(auto n : neigh){
                if(n != null)
                    new_adj.push_back(n);
            }
            gp.push_back(new_adj);
        }
    }
    g = gp;
    return tails;
}