library(ggplot2)

lines <- read.csv("/Users/andreacacioli/Desktop/NthRootResults.csv")

lines_no_result <- lines[,1:3]
lines_no_result$Root <- paste0(lines_no_result$Root, "th Root")

ggplot(lines_no_result, aes(x = Input, y = Time.In.Microseconds, color = Root)) + geom_smooth() +
  labs(title = "Time taken for the computation of the nth root over the size of the input", 
       x="Input", 
       y = "Time (µs)")

