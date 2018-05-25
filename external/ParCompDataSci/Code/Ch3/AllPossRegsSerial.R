# regresses response variable Y column against 
# all possible subsets of the Xi predictor variables, 
# with subset size up through k; returns the
# adjusted R-squared value for each subset

# arguments:
#    x:  matrix of predictors, one per column
#    y:  vector of the response variable
#    k:  max size of predictor set
# return value:
#    R matrix, showing adjusted R-squared values, 
#    indexed by predictor set

apr <- function(x,y,k,reverse=F,dyn=F,chunk=1) {
   p <- ncol(x)
   # generate matrix of predictor subsets
   allcombs <- genallcombs(p,k)
   ncombs <- length(allcombs)
   dochunk(1,x,y,allcombs,ncombs)
}

# generate all nonempty subsets of 1..p of size <= k; 
# returns a list, one element per predictor set
genallcombs <- function(p,k) {
   allcombs <- list()
   for (i in 1:k) {
      tmp <- combn(1:p,i)
      allcombs <- c(allcombs,matrixtolist(tmp,rc=2))
   }
   allcombs
}

# extracts rows (rc=1) or columns (rc=2) of a matrix, producing a list
matrixtolist <- function(rc,m) {
   if (rc == 1) {
      Map(function(rownum) m[rownum,],1:nrow(m))
   } else Map(function(colnum) m[,colnum],1:ncol(m))
}

# process all the predictor sets in the chunk 
# whose first index is psetstart
dochunk <- function(psetstart,x,y,allcombs,chunk) {
   ncombs <- length(allcombs)
   lasttask <- min(psetstart+chunk-1,ncombs)
   t(sapply(allcombs[psetstart:lasttask],do1pset,x,y))
}

# find the adjusted R-squared values for the given 
# predictor set index
do1pset <- function(onepset,x,y) {
   slm <- summary(lm(y ~ x[,onepset]))
   # form the report for this predictor set; need trailngs 0s so as to
   # form matrices of uniform numbers of rows, to use rbind() in
   # snowapr()
   n0s <- ncol(x) - length(onepset)
   c(slm$adj.r.squared,onepset,rep(0,n0s))
}

# predictor set seems best
test <- function(cls,n,p,k,chunk=1,dyn=F,rvrs=F) {
   gendata(n,p)
   apr(cls,x,y,k,rvrs,dyn,chunk)
}

gendata <- function(n,p) {
   x <<- matrix(rnorm(n*p),ncol=p)
   y <<- x%*%c(rep(0.5,p)) + rnorm(n)
}
