#' Return a graph with given size and edge distribution.
#' Is inputGenerator when provided with distrSampler
#' @param params - should contain a function distrSampler(size) returning vector of size samples from the desired
#' degree distribution
#' @param size - how many vertices should the graph have? 
graphGenerator <- function(size, params) {
  require(igraph)
  require(magrittr)
  new_degs <-  params$distrSampler(size) %>% round
  new_degs[new_degs > size] <- size # trim meaningless values
  if(sum(new_degs) %% 2 != 0){ new_degs[1] <- new_degs[1] + 1} # undirected graph must have even sum of orders
  g5 <- sample_degseq(out.deg = new_degs) %>%
    as.directed(mode = "arbitrary") %>%  # generated undirected graph! Fix?
    simplify(remove.loops = T)  # remove edges with from=to
  g5
}