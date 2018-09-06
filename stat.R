#!/usr/bin/env Rscript
args = commandArgs(trailingOnly=TRUE)

library(xml2)
library(XML)
library(plyr)
library(stringr)


csv <- args[1]

data <- read.csv(csv,sep=",", comment.char = '#', header = T);

n <- nrow(data)

med <- median(data$iteration)
sd <- sd(data$iteration)
mean <- mean(data$iteration)
max <- max(data$iteration)
min <- min(data$iteration)
var <- var(data$iteration)

res <- paste("median=",med,", sd=",sd,", mean=",mean," variance=",var,", max=",max,", min=",min)
print(res)
