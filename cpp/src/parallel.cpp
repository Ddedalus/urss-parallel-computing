#include "parallel.h"

using namespace std;

uint64_t sspParaBitset(vecGraph &g, int maxThreads)
{
    omp_set_num_threads(maxThreads);
    // Sum of distances
    uint64_t sum = 0;
    // Initialise reaching vector for each node
    vector<Bitset> reaching(g.nodes(), Bitset(g.nodes()));
    vector<Bitset> reachingNext(g.nodes(), Bitset(g.nodes()));
#pragma omp parallel
#pragma omp single
    cout << "Working with " << omp_get_num_threads() << " threads." << endl;
    {
#pragma omp for
        for (uint i = 0; i < g.nodes(); i++)
        {
            reaching[i].set(i, true);
            reachingNext[i].set(i, true);
        }

        // Distance increases on each iteration
        uint32_t dist = 1;

        bool done = false;
        while (!done)
        {
            // For each node
#pragma omp reduction(+ \
                      : sum)
            for (uint i = 0; i < g.nodes(); i++)
            {
                // For each neighbour
                for (auto n : g[i])
                {
                    reachingNext[i] += reaching[n];
                }
                // add the shortest paths found
                sum += dist * reachingNext[i].diff_size(reaching[i]);
            }
            // For each node, update reaching vector
            done = true;
#pragma omp reduction(&& \
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
#pragma omp single
            dist++;
        }
    }
    return sum;
}
