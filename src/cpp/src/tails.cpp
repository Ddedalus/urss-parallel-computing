#include "tails.h"
#include "algorithms.h"

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

uint64_t vertexTailsBFS(graph &g, unordered_map<nodeId, vector<uint32_t>> tails, nodeId vid){
    node v = g[vid];
    uint64_t sum = 0, count = 1;
    for(auto t : tails[vid])    // tails rooted at source
        sum += t*(t+1)/2;
    
	vector<bool> visited(g.size(), false);
	deque<qNode> q;
	q.push_back({v, 0}); visited[v.pos] = true;
	while(!q.empty() && count < g.size()){
		qNode current = q.front(); q.pop_front();
		for(auto n : current.n.neigh){
            node ne = g[n];
			if(! visited[ne.pos]){
				q.push_back({ne, current.dist + 1});
				sum += current.dist + 1;
                for(auto t : tails[n]){ // tails rooted at ne
                    sum += (current.dist+1)*t + t*(t+1)/2;
                }
				visited[ne.pos] = true;
                count++;
			}
		}
	}
	return sum;
}

inline uint32_t tailContribution(uint32_t t, uint32_t n, uint64_t ssp){
    return t * ssp + ((n - 2 * t)*t*(t+1))/2 + (t*(t+1)*(t-1))/3;
}

uint64_t sspBFStails(const graph& g){
    auto gp = g;
    auto tails = cutTails(gp);
    assignPositions(gp);
	uint64_t sum = 0;
	for(auto iter = g.begin(); iter != g.end(); ++iter){
	    auto ssp = vertexTailsBFS(gp, tails, iter->first);
        for(auto t : tails[iter->first]){    // SSP calculated from these tails
            auto tc = tailContribution(t, g.size(), ssp);
            sum += tc;
        }
        sum += ssp;
	}
	return sum;
}
