library(here)
library(igraph)
g <- read.graph(here("external", "SNAP", "web-Google.txt"), format = "edgelist")
d <- g %>% degree
new_g <- delete.vertices(g, which(d==0))
write.graph(new_g, here("external", "SNAP", "web-cleaned.txt"), format="edgelist")
