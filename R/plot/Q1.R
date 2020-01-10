# Read data
data<-read.table("/Users/quyixiang/Desktop/生物统计/lab/lab4/brca-rsem-fpkm-tcga-t-lab.txt", header = T)

# Extract data of gene KIF1C
gene<-as.numeric(as.matrix(data[1,2:ncol(data)]))
#第一问
# Use descriptive statistics in R to describe the FPKM of KIF1C gene
# Average
mean(gene) # 1116.575
#Range
range(gene) # 185.11 6294.04
# Standard deviation
sd(gene) # 500.466
# Median
median(gene) # 1051.79
# Summary 
summary(gene)
# Min. 1st Qu.  Median    Mean 3rd Qu.    Max. 
# 185.1   808.0  1051.8  1116.6  1359.6  6294.0 
# Kurtosis and Skewness
kurtosis(gene)
skewness(gene)
#第二问
#samplesize 200
mean_gene<-mean(sample(gene,200)) #  1146.288
#第三问
library(BSDA)
sample<-sample(gene,200)
mean(sample)
z<-z.test(sample,alternative = "two.sided", 
          mu = mean(gene), 
          sigma.x = sd(gene), 
          conf.level = 0.95)
z
#第四问
#boxplot
sample1<-sample(gene,200,replace = F)
sample2<-sample(gene,200,replace = F)
sample3<-sample(gene,200,replace = F)
temp<-data.frame(cbind(group=c(rep("first",200),rep("second",200),rep("third",200)),sample=c(sample1,sample2,sample3)))
temp <- data.frame("group"=temp$group,"sample"=as.numeric(as.character(temp$sample)))
plot_boxplot <- ggplot(temp, aes(y =sample,fill = group))
plot_boxplot + 
  geom_boxplot() +
  geom_hline(yintercept = mean(gene), linetype="dashed", color = "black", size=0.5) +
  labs(
    title="Boxplot for the three times of sampling",y="FPKM"
  )+ylim(0,3000)
