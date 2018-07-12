#include "tails.h"
#include "algorithms.h"

using namespace std;

// returns a map from node Id to a list of tail lengths
tailsMap cutTails(mapGraph &g)
{
    tailsMap tails;
    nodeId null = g.getEraseMark();
    auto count = 0;
    for (auto &pair : g)
    {
        if (pair.second.size() != 1)
            continue;

        int t = 1; //length of the tail
        nodeID previousId = pair.first, currentId = pair.second[0];
        pair.second[0] = null;
        Node *cur = &g[currentId];
        while (g[currentId].size() == 2)
        {
            t++;
            // select the other neighbour
            nodeId next = ((*cur)[0] == previousId) ? (*cur)[1] : (*cur)[0];
            (*cur)[0] = null;
            (*cur)[1] = null; //remove both edges
            previousId = currentId;
            currentId = next;
            cur = &g[currentId];
        }
        count += t;
        *find(cur->begin(), cur->end(), previousId) = null; // mark root edge as removed
        tails[currentId].push_back(t);
    }
    tails.reserve(tails.size());

    // cout<<"Removing " << count << " vertices as tail members" << endl;
    //clean up g:
    g.eraseMarked();
    return tails;
}

uint64_t vertexTailsBFS(vecGraph &g, tailsMap tails, nodeID vid)
{
    uint64_t sum = 0, count = 1;
    for (auto t : tails[vid]) // tails rooted at source
        sum += t * (t + 1) / 2;

    vector<bool> visited(g.nodes(), false);
    deque<qNodeLight> q;
    q.push_back({vid, 0});
    visited[vid] = true;
    while (!q.empty() && count < g.nodes())
    {
        auto current = q.front();
        q.pop_front();
        for (auto n : g[current.n])
        {
            if (!visited[n])
            {
                q.push_back({n, current.dist + 1});
                sum += current.dist + 1;
                for (auto t : tails[n])
                { // tails rooted at ne
                    sum += (current.dist + 1) * t + t * (t + 1) / 2;
                }
                visited[n] = true;
                count++;
            }
        }
    }
    return sum;
}

inline uint32_t tailContribution(uint32_t t, uint32_t n, uint64_t ssp)
{
    return t * ssp + ((n - 2 * t) * t * (t + 1)) / 2 + (t * (t + 1) * (t - 1)) / 3;
}

tailsMap transfromTails(tailsMap &tails, transformMap &tr)
{
    tailsMap ret;
    ret.reserve(tails.size());
    for (auto &pair : tails)
    {
        for (uint i = 0; i < pair.second.size(); i++)
        {
            ret[tr[i]].push_back(tr[pair.second[i]]);
        }
    }
    return ret;
}

uint64_t sspBFStails(const mapGraph &g)
{
    auto gp = g;
    tailsMap tails = cutTails(gp);
    transformMap tr;
    vecGraph vg(gp, tr);
    auto nTails = transfromTails(tails, tr);
    uint64_t sum = 0;
    for (uint i = 0; i < vg.nodes(); i++)
    {
        auto ssp = vertexTailsBFS(vg, nTails, i);
        for (auto t : tails[i])
        { // SSP calculated from these tails
            auto tc = tailContribution(t, g.nodes(), ssp);
            sum += tc;
        }
        sum += ssp;
    }
    return sum;
}
