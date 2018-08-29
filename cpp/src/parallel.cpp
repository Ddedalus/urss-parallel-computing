#include "parallel.h"
#include "algorithms.h"

using namespace std;

uint64_t sspParaBitset(const vecGraph &g, int maxThreads)
{
    omp_set_num_threads(maxThreads);
    // Sum of distances
    uint64_t sum = 0;
    bool done = false;
    // Initialise reaching vector for each node
    vector<Bitset> reaching(g.nodes(), Bitset(g.nodes()));
    vector<Bitset> reachingNext(g.nodes(), Bitset(g.nodes()));

#pragma omp parallel for
        for (uint i = 0; i < g.nodes(); i++)
        {
            reaching[i].set(i, true);
            reachingNext[i].set(i, true);
        }

        // Distance increases on each iteration, thread private variable
        uint32_t dist = 1;

        while (!done)
        {
            // For each node
#pragma omp parallel for reduction(+ \
                          : sum) schedule(static, 250)
            for (uint i = 0; i < g.nodes(); i++)
            {
                // For each neighbour
                for (auto n : g.neighbours(i))
                {
                    reachingNext[i] += reaching[n];
                }
                // add the shortest paths found
                sum += dist * reachingNext[i].diff_size(reaching[i]);
            }
            // For each node, update reaching vector
            done = true;
#pragma omp parallel for reduction(&& \
                          : done)
            for (uint i = 0; i < g.nodes(); i++)
            {
                reaching[i].copy_from(reachingNext[i]);
                if (done && !reaching[i].is_full())
                {
                    done = false;
                    // cout<<"Node "<< i <<" not full, size: "<< reaching[i].count()<<endl;
                }
            }
            dist++;
        }
    return sum;
}

uint64_t sspParaBFS(const vecGraph &g, int maxThreads)
{
    uint64_t sum = 0;
    omp_set_num_threads(maxThreads);
#pragma omp parallel
#pragma omp for reduction(+ \
                          : sum)
    for (uint i = 0; i < g.nodes(); i++)
    {
        sum += vertexBFS(g, i);
    }
    return sum;
}