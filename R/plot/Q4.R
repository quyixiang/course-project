#第一问
library(moments)
control <- read.csv("/Users/quyixiang/Desktop/生物统计/lab/lab4/brca-rsem-fpkm-tcga-lab.txt", header = T, sep = "\t")
swap70 <- subset(control, control$Hugo_Symbol == "SWAP70")
swap70 <- as.vector(t(swap70[,-1]))
library(psych)
print(describe(swap70))

#第二问
sample_health_50<-sample(swap70,50)
mean_50<-mean(sample_health_50)

#第三问
mean_all <- mean(swap70)
sd<-sd(swap70)
t_result_50 <- t.test(sample_health_50, mu = mean_all)
pvalue_50 <- t_result_50$p.value

#第四问
sample_health_70 <- sample(swap70, 70)
mean_70 <- mean(sample_health_70)
t_result_70 <- t.test(sample_health_70, mu = mean_all)
pvalue_70 <- t_result_70$p.value

sample_health_80 <- sample(swap70, 80)
mean_80 <- mean(sample_health_80)
t_result_80 <- t.test(sample_health_80, mu = mean_all)
pvalue_80 <- t_result_80$p.value

pvalue_50
pvalue_70
pvalue_80

#作图
library(ggplot2)

difference_50<-(mean_50-mean_all)/sd
difference_70<-(mean_70-mean_all)/sd
difference_80<-(mean_80-mean_all)/sd

diff=data.frame(name=c("50","70","90"),value=c(difference_50,difference_70,difference_80),col=c("red","orange","green"))

swap70_frame<-data.frame(swap70)
swap70_frame$swap70=(swap70_frame$swap70-mean_all)/sd
ggplot(data = NULL)+
  geom_histogram(data = swap70_frame,mapping = aes(x=swap70,y=..density..),alpha=0.3,col='black',fill='#009ddF',binwidth = 0.25)+
  geom_density(data = swap70_frame,mapping = aes(x=swap70),col='white',alpha=0.5,fill='#03970b')+
  labs(x='difference',y='Density')+
  geom_vline(xintercept = diff$value,col=diff$col,linetype=1,size=1,show.legend=NA)+
  geom_ribbon(data = NULL,mapping = aes(x = c(-3.8,qnorm(0.025)),ymin=0,ymax=0.45),fill='orange',alpha=0.2)+
  geom_ribbon(data = NULL,mapping = aes(x=c(qnorm(0.975),3.8),ymin=0,ymax=0.45),fill='orange',alpha=0.2)
