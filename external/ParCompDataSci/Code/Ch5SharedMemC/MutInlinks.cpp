// MutInlinks.cpp:  mutual in-links prevalence computation

// input is a graph adjacency matrix, element (i,j) being 1 or 0,
// depending on whether there is an edge from vertex i to vertex j

// we find the average number of mutual in-links, over all possible
// pairs of vertices

#include <Rcpp.h>
#include <omp.h>

// count number of mutual inlinks among vertices j > i with vertex i, in
// the matrix xa of type Rcpp::NumericMatrix 
int do_one_i(Rcpp::NumericMatrix xa,int i) 
{
   int nr = xa.nrow();
   int nc = xa.ncol();
   int sum = 0,j,k;
   if (i >= nc - 1) return 0;
   for (j = i+1; j < nc; j++) {
      for (k = 0; k < nr; k++) 
         sum += xa(k,i) * xa(k,j);
   }
   return sum;
}

RcppExport SEXP ompmutin(SEXP adj, SEXP nth)
{
   Rcpp::NumericMatrix xadj(adj);
   int nr = xadj.nrow();
   int nc = xadj.ncol();

   // set number of threads
   int nthreads = INTEGER(nth)[0];
   omp_set_num_threads(nthreads);

   // simplest approach
   int tot,i;
   #pragma omp parallel for reduction(+:tot)
   for (i = 0; i < nc; i++)
      tot += do_one_i(xadj,i);
   return Rcpp::wrap(tot);
}
