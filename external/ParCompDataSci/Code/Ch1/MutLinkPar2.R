library(parallel)

jloop <- function(ichunk) {
   tot <- 0
   nr <- nrow(lnks)  # lnks global at worker
   for (i in ichunk) {
      tmp <- lnks[(i+1):nr,] %*% lnks[i,]
      tot <- tot + sum(tmp)
   }
   tot
}

mutoutpar <- function(cls) {
   nr <- nrow(lnks)  # lnks global at manager
   clusterExport(cls,"lnks")
   ichunks <- 1:(nr-1)
   tots <- clusterApply(cls,ichunks,jloop)
   Reduce(sum,tots) / nr
}

sim <- function(nr,nc,cls) {
   lnks <<- matrix(sample(0:1,(nr*nc),replace=TRUE),nrow=nr)
   print(system.time(mutoutpar(cls)))
}

# set up a cluster of nworkers workers on a multicore machine
initmc <- function(nworkers) {
   makeCluster(nworkers)
}

# set up a cluster on machines specified, one worker per machine
initcls <- function(workers) {
   makeCluster(spec=workers)
}
