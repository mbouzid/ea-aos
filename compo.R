#!/usr/bin/env Rscript
args = commandArgs(trailingOnly=TRUE)

library(xml2)
library(XML)
library(plyr)

library(ggplot2)
library(scales)
library(grid)



csv <- args[1]

data_csv <- read.delim(csv,sep=",", comment.char = '#', header = T);

data <- subset(data_csv, select = -c(1) )
x <- data_csv[1]

n <- ncol(data)



matplot(x,data,col=rainbow(n),type="l",lty=1,xlab="Fitness",ylab="Population size")
legend("topright", inset=.05, legend=colnames(data), col=1:n, horiz=F,fill=rainbow(n))

