#' Return a graph with given size and edge distribution
#' Is an inputGenerator for Time Machine
#' @param distrfit - return value of fitdistrplu::fitdistr call. This function only requires fields "distname" and "estimate"
#' @param size - how many vertices should the graph have? 
#' 
#' @export
graphFromDist <- function(size, distrfit) {
  rdist <- paste0("r", distrfit$distname) %>% get # name of the sampling function
  new_degs <-  do.call(rdist, args = as.list(c(size, distrfit$estimate))) %>% round # sample degree of each node
  new_degs[new_degs > size] <- size # trim values exceeding order
  if(sum(new_degs) %% 2 != 0){ new_degs[1] <- new_degs[1] + 1} # undirected graph must have even sum of orders
  g5 <- sample_degseq(out.deg = new_degs) %>%
    as.directed(mode = "arbitrary") %>%  # generated undirected graph! Fix?
    simplify(remove.loops = T)  # remove edges with from=to
  g5
}