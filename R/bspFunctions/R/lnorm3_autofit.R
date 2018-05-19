#' fit lognormal distribution with a shift parameter
#' @return fit class from fitdistrplus. Use return$estimate to obtain fitted parameters. See ?fitdistrplus for more info
lnorm3_autofit <- function(samples) {
  require(fitdistrplus); require(EnvStats); require(stats)
  init_vals <- fitdist(samples + 1, "lnorm")$estimate
  lnorm_fit <- fitdist(samples + 1, "lnorm3", start = c(init_vals, threshold=0))
  lnorm_fit$estimate["threshold"] <- lnorm_fit$estimate["threshold"] - 1
  lnorm_fit
}