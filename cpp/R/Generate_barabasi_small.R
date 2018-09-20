library(here)
library(igraph)
library(stringr)

getTailedGraph <- function(nodes.count) {
  barabasi.game(nodes.count, power = 0.1, m = 10, out.dist = NULL, out.seq = NULL, out.pref = FALSE, zero.appeal = 1, directed = FALSE,
algorithm ="psumtree", start.graph = NULL);
}
barabasiPath <- here("data", "barabasi", "small")
system(paste("rm -R", barabasiPath))
system(paste("mkdir", "-p", barabasiPath))
set.seed(122)
for(t in c(250, 500, 1000, 1500, 2000, 3000, 5000)){
  b <- getTailedGraph(t);
  write.graph(b, file =paste0(barabasiPath,  "/", str_pad(t, 4, pad = "0"), ".edges"), format = "edgelist")
}