#第一问
library(ggplot2)

cal_p_valuie<-function(x,difference)
{
  re<-0
  for(i in x)if(abs(i)>difference)re<-re+1
  return(re/length(x))
}

difference<-400/982-35/110
p<-(400+35)/(982+110)
tmpdata1<-rbinom(n = 1000,size = 982,prob = p)/982
tmpdata2<-rbinom(n = 1000,size = 110,prob = p)/110
mydata<-data.frame(dd=tmpdata1-tmpdata2)
pvalue<-cal_p_valuie(mydata$dd,difference)
mydata$dd<-mydata$dd/sqrt(p*(1-p)*(1/982+1/110))
difference<-difference/sqrt(p*(1-p)*(1/982+1/110))

ggplot(data = NULL)+
  geom_histogram(data = mydata,mapping = aes(x=dd,y=..density..),alpha=0.3,col='black',fill='#009ddF',binwidth = 0.25)+
  geom_density(data = mydata,mapping = aes(x=dd),col='white',alpha=0.5,fill='#03970b')+
  labs(x='P1-P2',y='Density')+
  geom_vline(xintercept = difference,col='red',linetype=1,size=1.2)+
  geom_ribbon(data = NULL,mapping = aes(x = c(-3.8,qnorm(0.025)),ymin=0,ymax=0.45),fill='orange',alpha=0.2)+
  geom_ribbon(data = NULL,mapping = aes(x=c(qnorm(0.975),3.8),ymin=0,ymax=0.45),fill='orange',alpha=0.2)

#第二问使用bash脚本
#usr/bin/bash
#sed -n '/MC4R/p' brca-rsem-fpkm-tcga-t-lab.txt > brca-t-lab.txt
#sed -n '/MC4R/p' brca-rsem-fpkm-tcga-lab.txt > brca-lab.txt

#第三问
library(moments)
health<-read.table("/Users/quyixiang/Desktop/生物统计/lab/lab4/brca-lab.txt",header=F,check.names=FALSE)
health<-as.numeric(c(t(health))[2:111])
patient<-read.table("/Users/quyixiang/Desktop/生物统计/lab/lab4/brca-t-lab.txt",header=F,check.names=FALSE)
patient<-as.numeric(c(t(patient))[3:983])
mean1=mean(health)
mean2=mean(patient)
med1=median(health)
med2=median(patient)
r1=range(health)
r2=range(patient)
v1=var(health)
v2=var(patient)
s1=sd(health)
s2=sd(patient)
summary(health)
summary(patient)
quantile(health)
quantile(patient)
table(health)
table(patient)
prop.table(health)
prop.table(patient)
fivenum(health)
fivenum(patient)
k1=kurtosis(health)
k2=kurtosis(patient)
sk1=skewness(health)
sk2=skewness(patient)
r_description=data.frame(Sort=c('Health','Patient'),mean=c(mean1,mean2),median=c(med1,med2),min=c(r1[1],r2[1]),min=c(r1[2],r2[2]),var=c(v1,v2),sd=c(s1,s2),kurtosis=c(k1,k2),skewness=c(sk1,sk2))
r_description=r_description[-3,]
print(r_description)

#第四问
library(vioplot)
library(ggplot2)
#par(mar=c(4,4,1,0))
#par(mfrow=c(1,1))

v=data.frame(MC4R=NA,classification=NA )
for (i in 1:length(patient)){
  v[i,1]=log(patient[i]+0.001,2)
  v[i,2]="patient"
}
for (i in (length(patient)+1):(length(patient)+length(health))){
  v[i,1]=log(health[i-length(patient)]+0.001,2)
  v[i,2]="healthy people"
}

ggplot(v, aes(1, MC4R, fill = classification)) + geom_split_violin() +
  xlab(" ") + ylab("MC4R") + 
  ggtitle("distribution of gene expression")+
  theme(legend.position="left", 
        axis.text.x=element_text(colour="black",size=14), 
        axis.text.y=element_text(size=14,face="plain"), 
        axis.title.y=element_text(size = 14,face="plain"),
        axis.title.x=element_text(size = 14,face="plain"), 
        plot.title = element_text(size=15,face="bold",hjust = 0.5))
