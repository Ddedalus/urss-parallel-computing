#' given a path to file with columns "from" and "to", returns igraph object
#' @export
readPOETSGraph <- function(path) {
  require(igraph)
  data <- read.table(path, sep=" ", skip=2)
  graph_from_data_frame(d = data, directed=T)
}