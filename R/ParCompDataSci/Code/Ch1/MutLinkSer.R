

mutoutser<- function(links) {
   nr <- nrow(links)
   nc <- ncol(links)
   tot = 0
   for (i in 1:(nr-1)) {
      for (j in (i+1):nr) {
         for (k in 1:nc) 
            tot <- tot + links[i,k] * links[j,k]
      }
   }
   tot / nr
}

mutoutser1<- function(links) {
   nr <- nrow(links)
   nc <- ncol(links)
   tot <- 0
   for (i in 1:(nr-1)) {
      tmp <- links[(i+1):nr,] %*% links[i,]
      tot <- tot + sum(tmp)
   }
   tot / nr
}

sim <- function(nr,nc) {
   lnk <- matrix(sample(0:1,(nr*nc),replace=TRUE),nrow=nr)
   print(system.time(mutoutser(lnk)))
   print(system.time(mutoutser1(lnk)))
}

