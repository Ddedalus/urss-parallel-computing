# finds distances between all possible pairs of rows in the matrix x and
# rows in the matrix y, as with pdist() but in parallel

# arguments:
#    cls:  cluster
#    x:  data matrix 
#    y:  data matrix 
#    dyn:  True means dynamic scheduling
#    chunk:  chunk size 
# return value:
#    full distance matrix

library(parallel)
library(pdist)

snowpdist <- function(cls,x,y,dyn=F,chunk=1) {
   nx <- nrow(x)
   ichunks <- npart(nx,chunk)
   dists <- 
      if (!dyn) { clusterApply(cls,ichunks,dochunk,x,y) 
      } else clusterApplyLB(cls,ichunks,dochunk,x,y) 
   tmp <- Reduce(c,dists)
   new("pdist", dist = tmp, n = nrow(x), p = nrow(y))
}

# process all rows in ichunk 
dochunk <- function(ichunk,x,y
) { require(pdist)
   pdist(x[ichunk,],y)@dist
}

# partition 1:m into chunks of approx. size chunk
npart <- function(m,chunk) {
   require(parallel)
   splitIndices(m,ceiling(m/chunk))
}
