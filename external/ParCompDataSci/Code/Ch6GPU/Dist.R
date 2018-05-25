
library(gputools)
n <- 1000
m <- matrix(runif(n^2),ncol=n)
system.time(dist(m))
system.time(gpuDist(m))

