// takes a graph adjacency matrix for a directed graph, and converts it
// to a 2-column matrix of pairs (i,j), meaning an edge from vertex i to
// vertex j; the output matrix must be in lexicographical order

#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

// transgraph() does this work
// arguments:
//    adjm:  the adjacency matrix (NOT assumed symmetric), 1 for edge, 0
//           otherwise; note: matrix is overwritten by the function
//    n:  number of rows and columns of adjm
//    nout:  output, number of rows in returned matrix
// return value: pointer to the converted matrix

// finds chunk among 0,...,n-1 to assign to thread number me among nth
// threads
void findmyrange(int n,int nth,int me,int *myrange)
{  int chunksize = n / nth;
   myrange[0] = me * chunksize;
   if (me < nth-1) myrange[1] = (me+1) * chunksize - 1;
   else myrange[1] = n - 1;
}

int *transgraph(int *adjm, int n, int *nout)
{  
   int *outm,  // to become the output matrix
       *num1s,  // i-th element will be the number of 1s in row i of adjm
       *cumul1s;  // cumulative sums in num1s
   #pragma omp parallel
   {  int i,j,m;
      int me = omp_get_thread_num(),
          nth = omp_get_num_threads();
      int myrows[2];
      int tot1s;
      int outrow,num1si;
      #pragma omp single
      { 
         num1s = malloc(n*sizeof(int)); 
         cumul1s = malloc((n+1)*sizeof(int)); 
      }
      // determine the rows in adjm to be handled by this thread
      findmyrange(n,nth,me,myrows);
      // now go through each row of adjm assigned to this thread,
      // recording the locations (column numbers) of the 1s; to save on
      // malloc() ops, reuse adjm, writing the locations found in row i
      // back into that row
      for (i = myrows[0]; i <= myrows[1]; i++) {
         tot1s = 0;  // number of 1s found in this row
         for (j = 0; j < n; j++) 
            if (adjm[n*i+j] == 1) {
               adjm[n*i+(tot1s++)] = j;
            }
         num1s[i] = tot1s;
      }
      // one thread will use nums1, set by all threads, so make sure
      // they're all done
      #pragma omp barrier  
      #pragma omp single
      {  
         cumul1s[0] = 0;  // cumul1s[i] will be tot 1s before row i of adjm
         // now calculate where the output of each row in adjm 
         // should start in outm
         for (m = 1; m <= n; m++) {
            cumul1s[m] = cumul1s[m-1] + num1s[m-1];
         }
         *nout = cumul1s[n];
         outm = malloc(2*(*nout) * sizeof(int));
      }
      // implied barrier after "single" pragam
      // now fill in this thread's portion
      for (i = myrows[0]; i <= myrows[1]; i++) {
         outrow = cumul1s[i];  // current row within outm
         num1si = num1s[i];
         for (j = 0; j < num1si; j++) {
            outm[2*(outrow+j)] = i;
            outm[2*(outrow+j)+1] = adjm[n*i+j];
         }
      }
   }
   // implied barrier after "parallel" pragma
   return outm;
}
