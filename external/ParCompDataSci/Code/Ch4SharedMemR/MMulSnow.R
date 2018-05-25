snowmmul <- function(cls,u,v) {
   require(parallel)
   idxs <- splitIndices(nrow(u),length(cls))
   mmulchunk <- function(idxchunk) u[idxchunk,] %*% v
   res <- clusterApply(cls,idxs,mmulchunk)
   Reduce(rbind,res)
}

testcmp <- function(cls,n) {  
   require(parallel)
   mgrinit(cls)  
   mgrmakevar(cls,"a",n,n)
   mgrmakevar(cls,"c",n,n)
   amat <- matrix(runif(n^2),ncol=n)  
   a[,] <- amat
   clusterExport(cls,"mmulthread")  
   print(system.time(clusterEvalQ(cls,mmul(a,a,c))))
   print(system.time(cmat <- snowmmul(cls,amat,amat)))  
}
