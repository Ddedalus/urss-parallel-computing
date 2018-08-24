require(here)
require(igraph)
require(stringr)

getTailedGraph <- function(thousands) {
  barabasi.game(1000*thousands, power = 0.1, m = 10, out.dist = NULL, out.seq = NULL, out.pref = FALSE, zero.appeal = 1, directed = FALSE,
algorithm ="psumtree", start.graph = NULL);
}
barabasiPath <- here("data", "barabasi")
system(paste("rm -R", barabasiPath))
system(paste("mkdir", barabasiPath))
set.seed(122)
for(t in c(1, 2, 4, 8, seq(10, 80, by=10))){
  b <- getTailedGraph(t);
  write.graph(b, file =paste0(barabasiPath,  "/", str_pad(t, 3, pad = "0"), "k.edges"), format = "edgelist")
}
