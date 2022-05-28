#lines_total <- rbind(lines, lines_bigger)
lines_total <- read.csv("/Users/andreacacioli/Desktop/results.csv")
mi <- min(lines_total$Input)
ma <- max(lines_total$Input)
mi
ma
log2(mi)
log2(ma)


seq_len <- lines_total[lines_total$isPrime == 1, 4]
min(seq_len)
max(seq_len)
mean(seq_len)
length(seq_len)
hist(seq_len, xlim = c(1,8), xlab = "Number of Subdivisions")


input_time <- lines_total[lines_total$isPrime == 1, c(1,2,4)]  #get primes
input_time <- input_time[seq(1,length(input_time$Input),100),] #get 1 prime every 100

#plot(input_time$Input, input_time$Time.In.Microseconds, ylab = "Time (µs)", xlab = "Input")


#plot(input_time$Input, input_time$Time.In.Microseconds, ylab = "Time (µs)", xlab = "Input", col = ifelse(input_time$Sequence.Length == 1, 'red', 
#                                                                                                  ifelse(input_time$Sequence.Length == 2, 'green', 
#                                                                                                  ifelse(input_time$Sequence.Length == 3, 'blue', 
#                                                                                                  'black')))

input_time <- lines_total[lines_total$isPrime == 1, c(1,2,4)]  #get primes
input_time <- input_time[seq(1,length(input_time$Input),100),] #get 1 prime every 100
library(ggplot2)
ggplot(input_time, aes(x = Input, y = Time.In.Microseconds)) + stat_smooth()


ggplot(input_time, aes(x = Sequence.Length, y = Time.In.Microseconds)) + stat_smooth()
cor(input_time$Sequence.Length, input_time$Time.In.Microseconds)

cor(input_time$Sequence.Length, input_time$Input)
ggplot(input_time, aes(x =  Sequence.Length, y = Input)) + stat_smooth()
