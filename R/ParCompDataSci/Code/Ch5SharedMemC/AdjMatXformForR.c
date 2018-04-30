#include <R.h>
#include <omp.h>
#include <stdlib.h>

// transgraph() does this work
// arguments:
//    adjm:  the adjacency matrix (NOT assumed symmetric), 1 for edge, 0
//           otherwise; note: matrix is overwritten by the function
//    np:  pointer to number of rows and columns of adjm
//    nout:  output, number of rows in returned matrix
//    outm:  the converted matrix

void findmyrange(int n,int nth,int me,int *myrange)
{  int chunksize = n / nth;
   myrange[0] = me * chunksize;
   if (me < nth-1) myrange[1] = (me+1) * chunksize - 1;
   else myrange[1] = n - 1;
}

void transgraph(int *adjm, int *np, int *nout, int *outm)
{  
   int *num1s,  // i-th element will be the number of 1s in row i of adjm
       *cumul1s,  // cumulative sums in num1s
       n = *np;
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
      findmyrange(n,nth,me,myrows);
      for (i = myrows[0]; i <= myrows[1]; i++) {
         tot1s = 0;  // number of 1s found in this row
         for (j = 0; j < n; j++) 
            if (adjm[n*j+i] == 1) {
               adjm[n*(tot1s++)+i] = j;
            }
         num1s[i] = tot1s;
      }
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
      }
      int n2 = n * n;
      for (i = myrows[0]; i <= myrows[1]; i++) {
         outrow = cumul1s[i];  // current row within outm
         num1si = num1s[i];
         for (j = 0; j < num1si; j++) {
            outm[outrow+j] = i + 1;
            outm[outrow+j+n2] = adjm[n*j+i] + 1;
         }
      }
   }
}
