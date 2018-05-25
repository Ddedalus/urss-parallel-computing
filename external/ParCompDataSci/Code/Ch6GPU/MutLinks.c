

// MutLinks.c:  finds mean number of mutual outlinks, 
// among all pairs of Web sites in our set

#include <stdio.h>
#include <stdlib.h>

int nummut(int *m, int n) {
   int i,j,k,sum = 0; 
   for (i = 0; i < n; i++) {  
      for (j = i+1; j < n; j++) {  // all rows j > i
         for (k = 0; k < n; k++)
            sum += m[n*k+i] * m[n*k+j];
      }
   }
   return sum;
}

int main(int argc, char **argv)
{  int n = atoi(argv[1]);  // number of vertices
    int *m; // matrix
    int msize = n * n * sizeof(int);  
    m = (int *) malloc(msize);  
    // as a test, fill matrix with random 1s and 0s
    int i,j;
    for (i = 0; i < n; i++) {
       m[n*i+i] = 0;
       for (j = 0; j < n; j++) {
          if (j != i) m[i*n+j] = rand() % 2;
       }
    }
    // float nn2 = n * (n-1) / 2;
    printf("mean = %f\n",nummut(m,n)/(float)((n*(n-1))/2));
}
