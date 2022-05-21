lines <- read.csv("/Users/andreacacioli/Desktop/AdvancedProbabilisticAlgorithmResults.csv")

seq_len <- lines[lines$isPrime == 1, 4]
min(seq_len)
max(seq_len)
mean(seq_len)
length(seq_len)
hist(seq_len, xlim = c(1,8), xlab = "Number of Subdivisions")


input_time <- lines[lines$isPrime == 1, c(1,2,4)]

plot(input_time$Input, input_time$Time.In.Microseconds, ylab = "Time (µs)", xlab = "Input", col = ifelse(input_time$Sequence.Length == 1, 'red', 
                                                                                                  ifelse(input_time$Sequence.Length == 2, 'green', 
                                                                                                  ifelse(input_time$Sequence.Length == 3, 'blue', 
                                                                                                  'black'))))
library(ggplot2)
#ggplot(input_time, aes(x = Input, y = Time.In.Microseconds)) + geom_point() + stat_smooth()

