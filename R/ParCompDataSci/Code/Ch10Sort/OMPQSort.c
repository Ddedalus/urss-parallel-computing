// OpenMP example program:  quicksort; not necessarily efficient

// exchange the elements pointed to by yi and yj
void swap(int *yi, int *yj) 
{  int tmp = *yi;
   *yi = *yj;
   *yj = tmp;
}

// consider the section of x from x[low] to x[high], comparing each
// element to the pivot, x[low]; keep shuffling this section of x until
// all the elements to the left of x[m] are <= the pivot, and all the ones
// to the right are >= the pivot
int *separate(int *x, int low, int high)
{  int i,pivot,m;
   pivot = x[low];  // would be better to take, e.g., median of 1st 3 elts
   swap(x+low,x+high);
   m = low;
   for (i = low; i < high; i++) {
      if (x[i] <= pivot) {
         swap(x+m,x+i);
         m += 1;
      }
   }
   swap(x+m,x+high);
   return m;
}

// quicksort of the array z, elements zstart through zend; set the
// latter to 0 and n-1 in first call, where n is the length of z;
// firstcall is 1 or 0, according to whether this is the first of the
// recursive calls
void qs(int *z, int zstart, int zend, int firstcall)
{  
   #pragma omp parallel
   {  int part;
      if (firstcall == 1) {
         #pragma omp single nowait
         qs(z,0,zend,0);
      } else {
         if (zstart < zend) {
            part = separate(z,zstart,zend);
            #pragma omp task
            qs(z,zstart,part-1,0);
            #pragma omp task
            qs(z,part+1,zend,0);
         }

      }
   }
}

// test code
main(int argc, char**argv)
{  int i,n,*w;
   n = atoi(argv[1]);
   w = malloc(n*sizeof(int));
   for (i = 0; i < n; i++) w[i] = rand();
   qs(w,0,n-1,1);
   if (n < 25) 
      for (i = 0; i < n; i++) printf("%d\n",w[i]);
}
