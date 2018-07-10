#include "tails.h"

using namespace std;

// returns a map from node Id to a list of tail lengths
unordered_map<nodeId, vector<uint32_t>> cutTails(graph& g){
    unordered_map<uint32_t, vector<uint32_t>> tails;
    nodeId null = g.size() + 1;
    auto count = 0;
    for(auto& pair : g){
        if(pair.second.size() != 1) continue;

        int t = 1;  //length of the tail
        nodeId previousId=pair.first, currentId = pair.second[0];
        node* current = &g[currentId];
        pair.second[0] = null;
        while(current->size() == 2){
            t++;
            // select the other neighbour
            nodeId next = ((*current)[0] == previousId) ? (*current)[1] : (*current)[0];
            (*current)[0] = null; (*current)[1] = null;   //remove edges at current
            previousId = currentId; currentId = next;
            current = &g[currentId];
        }
        count += t;
        *find(current->neigh.begin(), current->neigh.end(), previousId) = null;
        // current is now the root:
        tails[currentId].push_back(t);
    }
    tails.reserve(tails.size());    // rehash to allow optimal capacity

    // cout<<"Removing " << count << " vertices as tail members" << endl;
    //clean up g:
    for(auto pair = g.begin(); pair != g.end();){
        if(pair->second.size() <= 2 && pair->second[0]==null){
            // cout << "Erasing vertex no "<< pair->first << endl;
            pair = g.erase(pair); // node in a tail, removing
        }
        else{   // other nodes may have some edges removed
            // cout << "Kept vertex no    "<< pair->first << endl;
            for(auto index = 0; index <  pair->second.size(); index++){
                if(pair->second[index] == null)
                    pair->second.removeEdge(index--);    // swaps elements so we shall not increment the index.
            }
            ++pair;
        }
    }
    return tails;
}