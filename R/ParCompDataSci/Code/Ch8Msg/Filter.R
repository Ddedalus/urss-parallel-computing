# Rmpi example:find set intersection

# for illustration purposes, not intended to be optimal

# arguments:

#    dta:  the data, consisting of 0s and 1s, so that dta[i,j] = 1 means
#          that the i-th observation 

pipefilter <- function(dta) {
   # send each worker its columns of dta
   for (i in 1:n) {
      if (i == 1) {
         mpi.send.Robj(dta[,1:2],tag=0,dest=1)
      }
   }
   mpi.bcast.Robj2slave(dowork)
   # start workers; note nonblocking call
   mpi.bcast.cmd(dowork,)
   # remove the evens right away
   odds <- seq(from=3,to=n,by=2)
   nodd <- length(odds)
   # send odds to node 1, in chunks 
   startmsg <- seq(from=1,to=nodd,by=msgsize)
   for (s in startmsg) {
      rng <- s:min(s+msgsize-1,nodd)
      mpi.send.Robj(odds[rng],tag=0,dest=1)
   }
   # send end-data sentinel
   mpi.send.Robj(NA,tag=0,dest=1)
   # receive results from last node
   lastnode <- mpi.comm.size()-1
   # return te result; don't forget the 2
   c(2,mpi.recv.Robj(tag=0,source=lastnode))
}

# worker code
dowork <- function(n,divisors,msgsize) {
   # which are my divisors?
   me <- mpi.comm.rank()
   lastnode <- mpi.comm.size()-1
   ld <- length(divisors)
   tmp <- floor(ld / lastnode)
   mystart <- (me-1) * tmp + 1
   myend <- mystart + tmp - 1
   if (me == lastnode) myend <- ld
   mydivs <- divisors[mystart:myend]
   if (me == lastnode) out <- NULL
   repeat {
      msg <- mpi.recv.Robj(tag=0,source=me-1)
      if (me < lastnode) {
         if (!is.na(msg[1])) {
            sieveout <- dosieve(msg,mydivs)
            mpi.send.Robj(sieveout,tag=0,dest=me+1)
         } else {  # no more coming in, so send sentinel
            mpi.send.Robj(NA,tag=0,dest=me+1)
            return()
         }
      } else {
         if (!is.na(msg[1])) {
            sieveout <- dosieve(msg,mydivs)
            out <- c(out,sieveout)
         } else {  # no more coming in, so send results to manager
            mpi.send.Robj(out,tag=0,dest=0)
            return()
         }
      }
   }
}

# check divisibility of the current chunk x
dosieve <- function(x,divs) {
   for (d in divs) {
      x <- x[x %% d != 0 | x == d]
   }
   x
}

# serial prime finder; can be used to generate divisor list of primepipe
serprime <- function(n) {
   nums <- 1:n
   x <- rep(1,n)
   maxdiv <- ceiling(sqrt(n))
   for (d in 2:maxdiv) {
      if (x[d])
         x[x !=0 & nums > d & nums %% d == 0] <- 0
   }
   nums[x != 0 & nums >= 2]
}
