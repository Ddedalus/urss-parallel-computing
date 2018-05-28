
#' Measure runtime of an algorithm over a series of generated inputs
#' 
#' Displays a progress bar of timeLimit consumption, updated every time algorithm returns.
#' Allows breaking the execution after predefined time.
#' 
#' @param algo a function to be benchmarked. Expected signature:
#' `algo(input, algArgs)` where input is returned from the inputGenerator.
#' Any output will be ignored (to be fixed in the future)
#' @param inputGenerator a function with signature: f(size, inputArgs) returning a single input for algo.
#' @param sizes vector of input sizes. Passed one by one to the inputGenerator.
#' @param timeLimit If that many seconds have passed, next size will not be evaluated. Note this doesn't interrupt
#' the algorithm
#' @param algArgs optional arguments to be passed to algo calls
#' @param inputArgs optional arguments to be passed to inputGenerator
#' 
#' @return A data frame containing size of the input and measured runtime. May store additional specification, too.
#' 
#' @export
runOnSizes <- function(algo, inputGenerator,
                     sizes = c(10, 100), timeLimit = 60,
                     algArgs = c(), inputArgs = NULL) {
  require(utils); require(igraph) # this should not be required here
  pbTime <- txtProgressBar(min=0, max = timeLimit, style=3)
  startTime  <- Sys.time()
  edges <- numeric(length = length(sizes)) # breaks generality of this function. Fix it later
  nodes <- numeric(length = length(sizes))
  elapsed <- numeric(length = length(sizes))
  runtime <- 0
  for(i in 1:length(sizes)){
    setTxtProgressBar(pbTime, runtime)
    g <- inputGenerator(sizes[i], inputArgs)
    elapsed[i] <- system.time(
      algo(g, algArgs)
    )["elapsed"]
    nodes[i] <- gorder(g)  # replace with a generic function call
    edges[i] <- gsize(g)
    runtime <- difftime(Sys.time(), startTime, units = "secs") 
    if(runtime > timeLimit){
      elapsed <- elapsed[1:i]
      nodes <- nodes[1:i]  # this renders difficult in implementation as generic
      edges <- edges[1:i]
      message("\nTimed out at i=", i)
      break
    }
  }
  return(data.frame(edges, nodes, elapsed))
  # add alg name, processors + architecture later
}