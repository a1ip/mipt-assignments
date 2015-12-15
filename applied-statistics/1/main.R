library(ggplot2) 

wm.test <- function(x, y, alternative = c("two.sided", "less", "greater")) {
  x1 <- sample(x, 2 * floor(length(x) / 2))
  y1 <- sample(y, 2 * floor(length(y) / 2))
  x_diff <- abs(x1[1 : (length(x1) / 2)] - x1[(length(x1) / 2 + 1) : length(x1)])
  y_diff <- abs(y1[1 : (length(y1) / 2)] - y1[(length(y1)/2 + 1) : length(y1)])
  return (wilcox.test(x_diff, y_diff, alternative))
}

m1 <- 0
sigma1 <- 1
m2 <- 0
sigma2 <- seq(from = 0.5, to = 2.0, by = 0.01)
n <- seq(from = 5, to = 50, by = 1)

generate_pvalue_data <- function(test) {
  data = cbind(expand.grid(sigma2, n), 0)
  colnames(data) <- c("sigma2", "n", "pvalue")
  for (i in 1 : nrow(data)) {
    sigma2 <- data[i, 1]
    n <- data[i, 2]
    first <- rnorm(n, m1, sigma1)
    second <- rnorm(n, m2, sigma2)
    data[i, 3] <- test(first, second)["p.value"]
  }
  return (data)
}

calculate_fo_data <- function(pvalue_data, alpha) {
  fo_data = cbind(expand.grid(sigma2, n), 0)
  colnames(fo_data) <- c("sigma2", "n", "fo")
  for (i in 1 : nrow(fo_data)) {
    if (pvalue_data[i, 3] < alpha) {
      fo_data[i, 3] <- 1
    }
  }
  return (fo_data)
}

current_pvalue = generate_pvalue_data(var.test)
ggplot(data = current_pvalue, aes(x = sigma2, y = n, fill = pvalue)) + geom_tile() + scale_fill_gradient(low = "black", high = "yellow")

current_pvalue = generate_pvalue_data(wm.test)
ggplot(data = current_pvalue, aes(x = sigma2, y = n, fill = pvalue)) + geom_tile() + scale_fill_gradient(low = "black", high = "yellow")

var_pvalue = cbind(rep(0, length(sigma2) * length(n)), 0, 0)
var_fo = cbind(rep(0, length(sigma2) * length(n)), 0, 0)
wm_pvalue = cbind(rep(0, length(sigma2) * length(n)), 0, 0)
wm_fo = cbind(rep(0, length(sigma2) * length(n)), 0, 0)

alpha = 0.05
repeats = 1000

for (i in 1 : repeats) {
  current_pvalue = generate_pvalue_data(var.test)
  var_pvalue <- var_pvalue + current_pvalue
  var_fo <- var_fo + calculate_fo_data(current_pvalue, alpha)
  current_pvalue = generate_pvalue_data(wm.test)
  wm_pvalue <- wm_pvalue + current_pvalue
  wm_fo <- wm_fo + calculate_fo_data(current_pvalue, alpha)
}
var_pvalue <- var_pvalue / repeats
var_fo <- var_fo / repeats
wm_pvalue <- wm_pvalue / repeats
wm_fo <- wm_fo / repeats

ggplot(data = var_pvalue, aes(x = sigma2, y = n, fill = pvalue)) + geom_tile() + scale_fill_gradient(low = "black", high = "yellow")
ggplot(data = var_fo, aes(x = sigma2, y = n, fill = fo)) + geom_tile() + scale_fill_gradient(low = "black", high = "yellow")
ggplot(data = wm_pvalue, aes(x = sigma2, y = n, fill = pvalue)) + geom_tile() + scale_fill_gradient(low = "black", high = "yellow")
ggplot(data = wm_fo, aes(x = sigma2, y = n, fill = fo)) + geom_tile() + scale_fill_gradient(low = "black", high = "yellow")

diff_pvalue <- var_pvalue
for (i in 1 : nrow(diff_pvalue)) {
  diff_pvalue[i, 3] <- diff_pvalue[i, 3] - wm_pvalue[i, 3]
}
ggplot(data = diff_pvalue, aes(x = sigma2, y = n, fill = pvalue)) + geom_tile() + scale_fill_gradient(low = "black", high = "yellow")

diff_fo <- var_fo
for (i in 1 : nrow(diff_fo)) {
  diff_fo[i, 3] <- diff_fo[i, 3] - wm_fo[i, 3]
}
ggplot(data = diff_fo, aes(x = sigma2, y = n, fill = fo)) + geom_tile() + scale_fill_gradient(low = "black", high = "yellow")