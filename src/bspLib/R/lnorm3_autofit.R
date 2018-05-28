#' fit lognormal distribution with a shift parameter
#' @return fit class from fitdistrplus. Use return$estimate to obtain fitted parameters. See ?fitdistrplus for more info
#' @export
lnorm3_autofit <- function(samples) {
  require(fitdistrplus); require(EnvStats); require(stats)
  init_vals <- fitdist(samples + 1, "lnorm")
  lnorm_fit  <-  tryCatch({
    fitdist(samples + 1, "lnorm3", start = c(init_vals$estimate, threshold=0))
  }, error = function(err) {
    message("Failed to estimate parameters, returning lnorm fit instead")
    return( NULL )
  })
  if(!is.null(lnorm_fit)){
    return(lnorm_fit)
  }
  else
    init_vals$estimate <- c(init_vals$estimate, threshold=1)
    init_vals$sd <- c(init_vals$sd, threshold=NaN)
    return(init_vals)
}