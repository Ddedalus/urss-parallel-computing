library(parallel)

jloop <- function(ichunk) {
   tot <- 0
   nr <- nrow(lnks)
   for (i in ichunk) {
      tmp <- lnks[(i+1):nr,] %*% lnks[i,]
      tot <- tot + sum(tmp)
   }
   tot
}

mutoutpar <- function(cls) {
   nr <- nrow(lnks)  # lnks global
   clusterExport(cls,"lnks")
   ichunks <- clusterSplit(cls,1:(nr-1))
   tots <- clusterApply(cls,ichunks,jloop)
   Reduce(sum,tots) / nr
}

# sim on matrix of size nr x nc, on cluster cls
sim <- function(nr,nc,cls) {
   lnks <<- matrix(sample(0:1,(nr*nc),replace=TRUE),nrow=nr)
   print(system.time(mutoutpar(cls)))
}

init <- function(nworkers) {
   makeCluster(nworkers) # return cluster object
}
