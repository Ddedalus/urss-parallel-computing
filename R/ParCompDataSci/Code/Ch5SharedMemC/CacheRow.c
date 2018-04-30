#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

// translate from 2-D to 1-D indices
int onedim(int n,int i,int j) {  return n * i + j;  }

void transp(int *m, int n)
{
   #pragma omp parallel 
   {  int i,j,tmp;
      // walk through all the above-diagonal elements, swapping them
      // with their below-diagonal counterparts
      #pragma omp for
      for (i = 0; i < n; i++) {
         for (j = i+1; j < n; j++) {
            tmp = m[onedim(n,i,j)];
            m[onedim(n,i,j)] = m[onedim(n,j,i)];
            m[onedim(n,j,i)] = tmp;
         }
      }
   }
}

int *m;

int main(int argc, char **argv)
{  int i,j;
   int n = atoi(argv[1]);
   m = malloc(n*n*sizeof(int));
   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         m[n*i+j] = rand() % 24;
   if (n <= 10) {
      for (i = 0; i < n; i++) {
         for (j = 0; j < n; j++) printf("%d ",m[n*i+j]);
         printf("\n");
      }
   }
   double startime,endtime;
   startime = omp_get_wtime();
   transp(m,n);
   endtime = omp_get_wtime();
   printf("elapsed time:  %f\n",endtime-startime);
   if (n <= 10) {
      for (i = 0; i < n; i++) {
         for (j = 0; j < n; j++) printf("%d ",m[n*i+j]);
         printf("\n");
      }
   }
}
