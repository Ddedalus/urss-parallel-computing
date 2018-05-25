# inputs a graph adjacency matrix, and outputs a two-column matrix
# listing the edges emanating from each node

library(Rdsm)

# arguments:
#    a:  adjacency matrix 
#    lnks:  edges matrix; shared, nrow(a)^2 rows and 2 columns
#    counts:  numbers of edges found by each thread; shared

# in this version, the matrix lnks must be created ahead of time; since
# the number of rows is uknown a priori, one must allow for the worst
# case, nrow(a)^2 rows; after the run, the number of actual rows will be
# in counts[1,length(cls)]

getlinksthread <- function(a,lnks,counts) {
   require(parallel)
   nr <- nrow(a)
   # get my assigned portion of a by splitting 1:m equally
   myidxs <- getidxs(nr)
   # Below: 1 means rows; rw is placeholder argument for this locally defined function;
   # which returns positions where condition is true
   myout <- apply(a[myidxs,],1,function(rw) which(rw==1))
   # myout[[i]] now lists the edges from node myidxs[1] + i - 1
   nmyedges <- Reduce(sum,lapply(myout,length))  # my total edges
   me <- myinfo$id
   counts[1,me] <- nmyedges
   barr()
   if (me == 1) {
      # use cumsum() to determine where each node will store its results
      # in lnks
      counts[1,] <- cumsum(counts[1,])
   }
   barr()
   # lnksidx will be the next row to write within lnks
   lnksidx <- if (me == 1) 1 else counts[1,me-1] + 1
   for (idx in myidxs)  {
      # corresponding index to idx within myout
      jdx <- idx - myidxs[1] + 1
      myoj <- myout[[jdx]]
      endwrite <- lnksidx + length(myoj) - 1
      if (!is.null(myoj)) {
         lnks[lnksidx:endwrite,] <- cbind(idx,myoj)
      }
      lnksidx <- endwrite + 1
   }
   0  # don't do expensive return of result
}

test <- function(cls) {
   mgrinit(cls)
   mgrmakevar(cls,"x",6,6)
   mgrmakevar(cls,"lnks",36,2)
   mgrmakevar(cls,"counts",1,length(cls))
   x[,] <- matrix(sample(0:1,36,replace=T),ncol=6)
   clusterExport(cls,"getlinksthread")
   clusterEvalQ(cls,getlinksthread(x,lnks,counts))
   print(lnks[1,counts[1,length(cls)],])
}
