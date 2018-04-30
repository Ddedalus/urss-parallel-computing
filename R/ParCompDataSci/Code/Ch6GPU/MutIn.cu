

// MutIn.cu:  finds mean number of mutual inlinks, 
// among all pairs of Web sites in our set; in checking 
// (i,j) pairs, thread k will handle all i such that 
// i mod totth = k, where totth is the number of threads

// usage:
//
//    mutin numvertices numblocks

#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>

// block size is hard coded as 192
#define BLOCKSIZE 192

// kernel:  processes all pairs assigned to a given thread
__global__ void procpairs(int *m, int *tot, int n)
{
   // total number of threads = number of blocks * block size
   int totth = gridDim.x * BLOCKSIZE,  
       // my thread number
       me = blockIdx.x * blockDim.x + threadIdx.x;  
   int i,j,k,sum = 0; 
   for (i = me; i < n; i += totth) {  // various columns i
      for (j = i+1; j < n; j++) {  // all columns j > i
         for (k = 0; k < n; k++)
            sum += m[n*k+i] * m[n*k+j];
      }
   }
   atomicAdd(tot,sum);
}

int main(int argc, char **argv)
{  int n = atoi(argv[1]),  // number of vertices
       nblk = atoi(argv[2]);  // number of blocks
    // the usual initializations
    int *hm, // host matrix
        *dm, // device matrix
        htot, // host grand total
        *dtot; // device grand total
    int msize = n * n * sizeof(int);  
    hm = (int *) malloc(msize);  
    // as a test, fill matrix with random 1s and 0s
    int i,j;
    for (i = 0; i < n; i++) {
       hm[n*i+i] = 0;
       for (j = 0; j < n; j++) {
          if (j != i) hm[i*n+j] = rand() % 2;
       }
    }
    // more of the usual initializations
    cudaMalloc((void **)&dm,msize);
    // copy host matrix to device matrix
    cudaMemcpy(dm,hm,msize,cudaMemcpyHostToDevice);
    htot = 0;
    // set up device total and initialize it
    cudaMalloc((void **)&dtot,sizeof(int));
    cudaMemcpy(dtot,&htot,sizeof(int),cudaMemcpyHostToDevice);
    // OK, ready to launch kernel, so configure grid
    dim3 dimGrid(nblk,1);
    dim3 dimBlock(BLOCKSIZE,1,1);
    // launch the kernel
    procpairs<<<dimGrid,dimBlock>>>(dm,dtot,n);
    // wait for kernel to finish
    cudaThreadSynchronize();
    // copy total from device to host
    cudaMemcpy(&htot,dtot,sizeof(int),cudaMemcpyDeviceToHost);
    // check results
    if (n <= 15) {
       for (i = 0; i < n; i++) {
          for (j = 0; j < n; j++) 
             printf("%d ",hm[n*i+j]);
          printf("\n");
       }
    }
    printf("mean = %f\n",htot/(float)((n*(n-1))/2));
    // clean up
    free(hm);
    cudaFree(dm);
    cudaFree(dtot);
}
