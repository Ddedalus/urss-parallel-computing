// OpenMP example program, Burst.c; burst() finds period of 
// highest burst of activity in a time series

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

// arguments for burst()

//    inputs:
//       x:  the time series, assumed nonnegative
//       nx:  length of x
//       k:  shortest period of interest
//    outputs:
//       startmax, endmax:  pointers to indices of the maximal-burst period
//       maxval:  pointer to maximal burst value

// finds the mean of the block between y[s] and y[e]
double mean(double *y, int s, int e) {
   int i; double tot = 0;
   for (i = s; i <= e; i++) tot += y[i];
   return tot / (e - s + 1);
}

void burst(double *x, int nx, int k, 
   int *startmax, int *endmax, double *maxval) 
   {
   int nth;  // number of threads
   #pragma omp parallel  
   {  int perstart, // period start
          perlen,  // period length
          perend,  // perlen end
          pl1;  // perlen - 1
      // best found by this thread so far
      int mystartmax, myendmax;  // locations
      double mymaxval;  // value
      // scratch variable
      double xbar;
      int me;  // ID for this thread
      #pragma omp single
      {
          nth = omp_get_num_threads();
      }
      me = omp_get_thread_num();
      mymaxval = -1;
      #pragma omp for
      for (perstart = 0; perstart <= nx-k; perstart++) {
         for (perlen = k; perlen <= nx - perstart; perlen++) {
            perend = perstart+perlen-1;
            if (perlen == k) 
               xbar = mean(x,perstart,perend);
            else  {
               // update the old mean
               pl1 = perlen - 1;
               xbar = (pl1 * xbar + x[perend]) / perlen;
            }
            if (xbar > mymaxval) {
               mymaxval = xbar;
               mystartmax = perstart;
               myendmax = perend;
            }
         }
      }
      #pragma omp critical
      {
         if (mymaxval > *maxval) {
            *maxval = mymaxval;
            *startmax = mystartmax;
            *endmax = myendmax;
         }
      }
   }
}

// here's our test code

int main(int argc, char **argv)
{  
   int startmax, endmax;
   double maxval;
   double *x;  
   int k = atoi(argv[1]);
   int i,nx;
   nx = atoi(argv[2]);  // length of x
   x = malloc(nx*sizeof(double));
   for (i = 0; i < nx; i++) x[i] = rand() / (double) RAND_MAX;
   double startime,endtime;
   startime = omp_get_wtime();
   // parallel
   burst(x,nx,k,&startmax,&endmax,&maxval);
   // back to single thread
   endtime = omp_get_wtime();
   printf("elapsed time:  %f\n",endtime-startime);
   printf("%d %d %f\n",startmax,endmax,maxval);
   if (nx < 25) {
      for (i = 0; i < nx; i++) printf("%f ",x[i]);
      printf("\n");
   }
}
