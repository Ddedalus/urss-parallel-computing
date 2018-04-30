#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

// translate from 2-D to 1-D indices
int onedim(int n,int i,int j) {  return n * i + j;  }

void trade(int *m,int n,int i,int j) {
   int tmp;
   tmp = m[onedim(n,i,j)];
   m[onedim(n,i,j)] = m[onedim(n,j,i)];
   m[onedim(n,j,i)] = tmp;
}

void transp(int *m, int n)
{  int n1 = n - 1;
   int n2 = 2 * n - 3;
   #pragma omp parallel 
   {  int w,j;
      int row, col;
      #pragma omp for
      // w is wavefront number, indexed across top row, bottom row
      // we move from northeast to southwest within diagonals
      for (w = 1; w <= n2; w++) {
         if (w < n) {
            row = 0;
            col = w;
         } else {
            row = w - n1;
            col = n1;
         }
         for (j = 0; ; j++) {
            if (row > n1 || col < 0) break;
            if (row >= col) break;
            trade(m,n,row++,col--);
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
