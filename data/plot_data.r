data <- read.csv("date_v_temp.csv")
y <- data$temp
x <- 1:length(y)

dev <- pdf("temp_plot.pdf", width=15, height=8)
plot(x,y,
     main="Temperature data Taourit Izakarn",
     xlab="day",
     ylab="temperature °C")
dev.off()
