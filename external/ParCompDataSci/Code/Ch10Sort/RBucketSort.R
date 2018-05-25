# mc.cores is the number of cores to use in computation
mcbsort <- function(x,ncores,nsamp=1000) {
   require(parallel)
   # get subsample to determine approximate quantiles
   samp <- sort(x[sample(1:length(x),nsamp,replace=TRUE)])
   # each thread will run dowork()
   dowork <- function(me) {
      # which numbers will this thread process?
      k <- floor(nsamp / ncores)
      if (me > 1) mylo <- samp[(me-1) * k]
      if (me < ncores) myhi <- samp[me * k]
      if (me == 1) myx <- x[x <= myhi] else 
         if (me == ncores) myx <- x[x > mylo] else
            myx <- x[ x > mylo & x <= myhi]
      # this thread now sorts its assigned group
      sort(myx)
   }
   res <- mclapply(1:ncores,dowork,mc.cores=ncores)
   # string the results together
   c(unlist(res))
}

test <- function(n,ncores) {
   x <- runif(n)
   mcbsort(x,ncores=ncores,nsamp=1000)
}
