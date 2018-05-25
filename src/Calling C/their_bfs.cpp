#include <Rcpp.h>
using namespace Rcpp;

// This is a simple example of exporting a C++ function to R. You can
// source this function into an R session using the Rcpp::sourceCpp 
// function (or via the Source button on the editor toolbar). Learn
// more about Rcpp at:
//
//   http://www.rcpp.org/
//   http://adv-r.had.co.nz/Rcpp.html
//   http://gallery.rcpp.org/
//

// Compute sum of all shortest paths
// [[Rcpp::export]]
uint64_t ssp()
{
  // Sum of distances
  uint64_t sum = 0;
  
  // Visited array
  uint8_t *visited = (uint8_t*) calloc(numNodes, sizeof(uint8_t));
  
  // Fringe array and size
  uint32_t *fringe = (uint32_t*) calloc(numNodes, sizeof(uint32_t));
  uint32_t fringeSize;
  
  // New fringe array and size
  uint32_t *newFringe = (uint32_t*) calloc(numNodes, sizeof(uint32_t));
  uint32_t newFringeSize;
  
  // Apply BFS from each vertex in turn
  for (uint32_t src = 0; src < numNodes; src++) {
    newFringeSize = 0;
    fringeSize = 1;
    fringe[0] = src;
    memset(visited, 0, numNodes);
    visited[src] = 1;
    
    uint32_t d = 1;
    while (fringeSize > 0) {
      // For each node in fringe
      for (uint32_t i = 0; i < fringeSize; i++) {
        uint32_t v = fringe[i];
        // For each neighbour
        uint32_t numNeighbours = neighbours[v][0];
        for (uint32_t j = 1; j <= numNeighbours; j++) {
          uint32_t n = neighbours[v][j];
          if (! visited[n]) {
            visited[n] = 1;
            newFringe[newFringeSize++] = n;
            sum += d;
          }
        }
      }
      // Increment distance and move to new fringe
      d++;
      uint32_t* p = fringe; fringe = newFringe; newFringe = p;
      fringeSize = newFringeSize; newFringeSize = 0;
    }
  }
  
  return sum;
}

// You can include R code blocks in C++ files processed with sourceCpp
// (useful for testing and development). The R code will be automatically 
// run after the compilation.
//

/*** R
timesTwo(42)
*/
