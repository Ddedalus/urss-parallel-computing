// AdjRcpp.cpp

#include <Rcpp.h>
#include <omp.h>

// the function transgraph() does the work
// arguments:
//    adjm:  the adjacency matrix (NOT assumed 
//           symmetric), 1 for edge, 0 otherwise; 
//            note: matrix is overwritten 
//           by the function
//  return value: the converted matrix

// finds the chunk of rows this thread will process
void findmyrange(int n,int nth,int me,int *myrange)
{  int chunksize = n / nth;
   myrange[0] = me * chunksize;
   if (me < nth-1) 
      myrange[1] = (me+1) * chunksize - 1;
   else myrange[1] = n - 1;
}

RcppExport SEXP transgraph(SEXP adjm)
{  
   int *num1s,  // i-th element will be the number 
                // of 1s in row i of adjm
       *cumul1s,  // cumulative sums in num1s
       n;
   Rcpp::NumericMatrix xadjm(adjm);
   n = xadjm.nrow();
   int n2 = n*n;
   Rcpp::NumericMatrix outm(n2,2);

   #pragma omp parallel
   {  int i,j,m;
      int me = omp_get_thread_num(),
          nth = omp_get_num_threads();
      int myrows[2];
      int tot1s;
      int outrow,num1si;
      #pragma omp single
      { 
         num1s = (int *) malloc(n*sizeof(int)); 
         cumul1s = (int *) malloc((n+1)*sizeof(int)); 
      }
      findmyrange(n,nth,me,myrows);
      for (i = myrows[0]; i <= myrows[1]; i++) {
         // number of 1s found in this row
         tot1s = 0;  
         for (j = 0; j < n; j++) 
            if (xadjm(i,j) == 1) {
               xadjm(i,(tot1s++)) = j;
            }
         num1s[i] = tot1s;
      }
      #pragma omp barrier  
      #pragma omp single
      {  
         // cumul1s[i] will be tot 1s before row 
         // i of xadjm
         cumul1s[0] = 0;  
         // now calculate where the output of each 
         // row in adjm should start in outm
         for (m = 1; m <= n; m++) {
            cumul1s[m] = cumul1s[m-1] + num1s[m-1];
         }
      }
      for (i = myrows[0]; i <= myrows[1]; i++) {
         // current row within outm
         outrow = cumul1s[i];  
         num1si = num1s[i];
         for (j = 0; j < num1si; j++) {
            outm(outrow+j,0) = i + 1;
            outm(outrow+j,1) = xadjm(i,j) + 1;
         }
      }
   }

   Rcpp::NumericMatrix outmshort = 
      outm(Rcpp::Range(0,cumul1s[n]-1),
         Rcpp::Range(0,1));
   return outmshort;
}
