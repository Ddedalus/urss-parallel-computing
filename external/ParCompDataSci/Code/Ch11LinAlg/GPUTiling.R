biggpuax <- function(a,x,ntiles)
{
   require(parallel)
   require(gputools)
   nrx <- nrow(a)
   y <- vector(length=nrx)
   tilesize <- floor(nrx / ntiles)
   for (i in 1:ntiles) {
      tilebegin <- (i-1) * tilesize + 1
      tileend <- i * tilesize
      if (i == ntiles) tileend <- nrx
      tile <- tilebegin:tileend
      y[tile] <- gpuMatMult(a[tile,,drop=FALSE],x)
   }
   y
}
