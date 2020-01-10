#第一问
brca_tumor <- read.table('/Users/quyixiang/Desktop/生物统计/lab/lab4/brca-rsem-fpkm-tcga-t-lab.txt',header = T)
brca_contrl <- read.table('/Users/quyixiang/Desktop/生物统计/lab/lab4/brca-rsem-fpkm-tcga-lab.txt',header = T)
brca_contrl_mean <- apply(subset(brca_contrl,select = 2:ncol(brca_contrl)),1,mean)
brca_tumor_mean <- apply(subset(brca_tumor,select = 2:ncol(brca_contrl)),1,mean)
brca_contrl_mean <- cbind(subset(brca_contrl,select=1),as.data.frame(brca_contrl_mean))
brca_tumor_mean <- cbind(subset(brca_tumor,select=1),as.data.frame(brca_tumor_mean))
brca_contrl_mean <- brca_contrl_mean[order(brca_contrl_mean$brca_contrl_mean,decreasing = T),]
brca_tumor_mean <- brca_tumor_mean[order(brca_tumor_mean$brca_tumor_mean,decreasing = T),]
#第二问
genes_in_control <- as.vector(head(brca_contrl_mean,500))
genes_in_tumor <- as.vector(head(brca_tumor_mean,500))
same_contrl_genes <- subset(genes_in_control, genes_in_control$Hugo_Symbol==genes_in_tumor$Hugo_Symbol)
same_tumor_genes <- subset(genes_in_tumor, genes_in_control$Hugo_Symbol==genes_in_tumor$Hugo_Symbol)
summary(genes_in_control$brca_contrl_mean)
summary(genes_in_tumor$brca_tumor_mean)
#第三问
same_contrl_genes <- data.frame(same_contrl_genes,Type = 'Normal')
same_tumor_genes <- data.frame(same_tumor_genes,Type='Tumor')
colnames(same_contrl_genes) <- c('Gene','Mean','Type')
colnames(same_tumor_genes) <- c('Gene','Mean','Type')
same_genes <- rbind(same_contrl_genes,same_tumor_genes)
t.test(same_contrl_genes$Mean,same_tumor_genes$Mean,paired = T,alternative = 'two.sided')
#第四问
library(ggplot2)
means <- c(mean(same_contrl_genes$Mean), mean(same_tumor_genes$Mean))
sds <- c(sd(same_contrl_genes$Mean), sd(same_tumor_genes$Mean))
mydf5_4 <- data.frame(Type = c("Normal", "Tumor"), Mean = means, S.D = sds)
ggplot(data = mydf5_4, aes(x = as.factor(Type), y = mydf5_4$Mean,fill=as.factor(Type)))+
  scale_fill_brewer(palette="Pastel2")+
  geom_bar(stat = "identity", position = "dodge",color = 'black',size =0.5)+
  geom_errorbar(stat = "identity",aes(ymin=Mean,ymax = Mean+S.D/sqrt(5)),width=0.2, size=0.5, color="black")+
  labs(title="Paired t-Test") +
  xlab("Type") + labs(fill = 'Type')+
  ylab("Gene Expression (in FPKM)") +
  annotate("text", x = 2, y = 3e5, label = "pvalue=0.0639")
