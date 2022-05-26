lines <- read.csv("/Users/andreacacioli/Desktop/AdvancedProbabilisticAlgorithmResults.csv")
lines_bigger <- read.csv("/Users/andreacacioli/Desktop/APA2.csv")

s <- min(lines$Input)
f<-max(lines_bigger$Input)

f-s

lines_total <- rbind(lines, lines_bigger)

seq_len <- lines_total[lines_total$isPrime == 1, 4]
min(seq_len)
max(seq_len)
mean(seq_len)
length(seq_len)
#hist(seq_len, xlim = c(1,8), xlab = "Number of Subdivisions")


input_time <- lines_total[lines_total$isPrime == 1, c(1,2,4)]
input_time <- lines_total[seq(1,length(lines_total$Input),25),]

plot(input_time$Input, input_time$Time.In.Microseconds, ylab = "Time (µs)", xlab = "Input")


#plot(input_time$Input, input_time$Time.In.Microseconds, ylab = "Time (µs)", xlab = "Input", col = ifelse(input_time$Sequence.Length == 1, 'red', 
                                                                                                  ifelse(input_time$Sequence.Length == 2, 'green', 
                                                                                                  ifelse(input_time$Sequence.Length == 3, 'blue', 
                                                                                                  'black')))

#library(ggplot2)
#ggplot(input_time, aes(x = Input, y = Time.In.Microseconds)) + stat_smooth()

