library(igraph)
library(here)
library(stringr)
load(here("src", "cpp", "R", "fit_data_for_newcastle.dat"))
lnormPath <- here("data", "lnorm")
source("graph_from_distr.R")
system(paste("rm -R", lnormPath))
system(paste("mkdir", lnormPath))
set.seed(229)
for(t in c(5, 10, seq(20, 80, by=20))){
  b <- graph_from_distr(1000*t, n5_lnorm_fit)
  write.graph(b, file = paste0(lnormPath, "/", str_pad(t, 3, pad = "0"), "k.edges"), format = "edgelist")
}