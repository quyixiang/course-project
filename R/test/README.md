#### 1.Breast cancer is the second most common cancer in women after skin cancer. When breast cancer is invasive, it starts in the breast ducts or glands but grows into breast tissue. It can then spread into the nearby lymph nodes and beyond. Researchers collected some invasive breast cancer samples and preformed RNA-seq analysis. Table1 contained the expression value of 100 genes in 60 tumor samples. The expression value represents as transformed z-score. The clinical information of the samples is shown in Table 2. The subtypes information can be obtained from PAM50_SUBTYPE column (namely luminal A, luminal B, HER2 enriched and basallike types). (alpha = 0.05)

##### (1) Is there any genes differentially expressed among these subtypes? ``one-way ANOVA``

###### 数据预处理

```R
# Load packages
library(dplyr)

# Input the data of gene expresssion(table 1) and information of patients(table 2)
gene_expression <- read.table("/Users/quyixiang/Desktop/生物统计/lab/lab5/Table 1.txt",sep = "\t",header = F)
patients <- read.table("/Users/quyixiang/Desktop/生物统计/lab/lab5/Table 2.txt",sep = "\t",header = T)

# Pre-processing of data
gene_expression <- t(gene_expression)
gene_expressions <- gene_expression[2:nrow(gene_expression),]
colnames(gene_expressions) = gene_expression[1,]
gene_expressions <- data.frame(gene_expressions)
for(i in 3:102){
  gene_expressions[,i] <- as.numeric(as.character(gene_expressions[,i]))
}
```

###### 正态性检验

```R
groups <- as.character(unique(gene_expressions$subtype))
isNormal <- rep(TRUE, length(3:102))
for(i in 3:102){
  if(!is.na(gene_expressions[,i][4])){
    for (j in 1:length(groups)) {
      sw.res <- shapiro.test(gene_expressions[which(gene_expressions$subtype==groups[j]), i])
      if (sw.res$p.value<=0.05) {
        isNormal[i-2] <- FALSE
        break
      }
    }
  }
}
summary(isNormal)
```

###### 方差齐性检验

```R
var.homo.pvalues <- c()
for(i in 3:102){
  if(!is.na(gene_expressions[,i][4])){
    temp <- data.frame(group = gene_expressions$subtype, 
                       expression = gene_expressions[,i])
    temp$group <- as.character(temp$group)
    b.res <- bartlett.test(expression~group, data=temp)
    var.homo.pvalues <- append(var.homo.pvalues, b.res$p.value)
  }else{
    var.homo.pvalues <- append(var.homo.pvalues,0)
  }
}
isHomo <- ifelse(var.homo.pvalues>0.05,TRUE,FALSE)
summary(isHomo)

# Summary the requirements
isSatisfied <- ifelse(isHomo==TRUE,isNormal,FALSE)
summary(isSatisfied)
```

###### ANOVA

```R
pvalues <- c()
for(i in 3:102){
  if(!is.na(gene_expressions[,i][4]) && isSatisfied[i-2]==TRUE){
    res.aov <- aov(gene_expressions[,i] ~ subtype, data = gene_expressions)
    pvalue <- summary(res.aov)[[1]][,5][1]
    pvalues <- append(pvalues,pvalue)
  }else{
    pvalues <- append(pvalues,1)
  }
}
tmp <- sort(pvalues,index.return = T)

# Find the gene whose pvalue <0.05
for(i in 1:100){if(tmp$x[i]>=0.05)break}
gene_different <- colnames(gene_expressions[3:102])[tmp$ix[1:i-1]]
paste(gene_different,collapse = ",")
```

###### 结果

![image-20191214101838287](/Users/quyixiang/Library/Application Support/typora-user-images/image-20191214101838287.png)

##### (2) Do pair-wised tests for the top 10 genes with most differential expression (smallest p values) ``paired t-test``

```R
# Paired t-test
subindex <- tmp$ix+2
gene_top <- gene_expressions[,subindex[1:10]]
gene_top <- cbind(gene_top,"subtype"= gene_expressions$subtype)

for (i in 1:10){
  print("--------------------------------------------------------")
  print(paste("The unpaired t-test for",colnames(gene_top)[i],"is"))
  print("Differential subtypes are:")
  LA <- gene_top[,i][1:15]
  LB <- gene_top[,i][16:30]
  Her2 <- gene_top[,i][31:45]
  Basal <- gene_top[,i][46:60]
  p1 <- t.test(LA, LB, paired = F)$p.value
  p2 <- t.test(LA, Her2, paired = F)$p.value
  p3 <- t.test(LA, Basal, paired = F)$p.value
  p4 <- t.test(LB, Her2, paired = F)$p.value
  p5 <- t.test(LB, Basal, paired = F)$p.value
  p6 <- t.test(Her2, Basal, paired = F)$p.value
  p_value <- c(p1, p2, p3, p4, p5, p6)
  p_value <- p.adjust(p_value, method="fdr", n=length(p_value))
  pair <- c('Luminal A and Luminal B', 'Luminal A and HER2-enriched', 'Luminal A and Basal-like',
            'Luminal B and HER2-enriched', 'Luminal B and Basal-like', 'HER2-enriched and Basal-like')
  for (i in 1:6){
    if (p_value[i] < 0.05){
      print(paste(pair[i], ' p-value= ', p_value[i]))
    }
  }
}
```

###### 结果

![image-20191217103356515](/Users/quyixiang/Library/Application Support/typora-user-images/image-20191217103356515.png)

#### 3.Colorectal cancer (CRC) is the third most common cancer worldwide and the fourth leading cause of cancer-related deaths. Recent studies of the genomic, transcriptomic, and proteomic landscapes of human CRC have identified many biological discoveries. However, the rapidly accumulating omics data have yet to bring novel biomarker and drug targets to the public. So researchers collected tumor specimens and matched normal adjacent tissues from some colon cancer patients. Next they performed wholeexome sequencing, copy-number array, RNA sequencing and microRNA sequencing, and label-free shotgun proteomic analyses of these samples. Table 4 and Table 5 are the log-ratio normalized proteome expression data in all tumors and matched normal adjacent tissues. (alpha = 0.05)

##### (1) Select the matched samples without missing value for all genes, and save the data into a new file.

```R
library(ggplot2)
tumors <- read.table("/Users/quyixiang/Desktop/生物统计/lab/lab5/Table 4.txt", header = T, check.names = F)
adjacents <- read.table("/Users/quyixiang/Desktop/生物统计/lab/lab5/Table 5.txt", header = T, check.names = F)
#将adjacents中编号的后缀“N”去掉
colnames(adjacents) <- gsub("N", "", colnames(adjacents))
#求两个数据集中匹配的患者编号
match_name <- intersect(colnames(tumors), colnames(adjacents))
#保存匹配患者数据集
new_tumors <- tumors[, colnames(tumors) %in% match_name]
write.table(new_tumors, file = "./match_tumors.txt", row.names = F, sep = "\t", quote = F)
new_adjacents <- adjacents[, colnames(adjacents) %in% match_name]
write.table(new_adjacents, file = "./match_adjacents.txt", row.names = F, sep = "\t", quote = F)
```

##### (2) Check the genes expressed differentially between tumor and the matched normal using nonparametric tests. ``Wilcoxon signed rank test``

```R
#wilcox符号秩和检验
#使用中位数补充缺失值
library(DMwR)
tumors <- knnImputation(new_tumors, k=10, meth = "median")
adjacents <- knnImputation(new_adjacents, k=10, meth = "median")
#对每个基因进行Wilcox符号秩和检验，并保存其p-value
p_values_1 <- c()
for (i in 1:8067) {
  z <- wilcox.test(x = unlist(matrix(tumors[i, -1])), 
                   y = unlist(matrix(adjacents[i, -1])), 
                   paired= TRUE)
  p_values_1 <- c(p_values_1, z$p.value)#写入p value
}

genes <- tumors[, 1]
genes_pvalue_1 <- data.frame(genes, p_values_1)
```



##### (3) Suppose the data are normally distributed, use t-test to do statistics again, and then compare the results with nonparametric method. Describe the differences. ``paired t-test``

###### 检验

```R
#配对t检验
p_values_2 <- c()
for (i in 1:8067) {
  z <- t.test(x = unlist(matrix(tumors[i, -1])), 
              y = unlist(matrix(adjacents[i, -1])), 
              paired= TRUE)
  p_values_2 <- c(p_values_2, z$p.value)#写入p value
}
#输出样本与对应的p-value
genes <- tumors[, 1]
genes_pvalue_2 <- data.frame(genes, p_values_2)
```

###### 作图

```R
sum(as.matrix(genes_pvalue_1$p_values_1) < 0.05)
sum(as.matrix(genes_pvalue_2$p_values_2) < 0.05)

dt<-data.frame(A=c(6741,1326),B=c('p<0.05','p>0.05'))
dt = dt[order(dt$A, decreasing = TRUE),] 
myLabel = as.vector(dt$B) 
myLabel = paste(myLabel, "(",dt$A,'  ', round(dt$A / sum(dt$A) * 100, 2), "%)", sep = "")
mynewLabel=as.vector(dt$B) 
ggplot(dt, aes(x = "", y = A, fill = B)) +
  geom_bar(stat = "identity", width = 10) +
  coord_polar(theta = "y") +
  labs(x = "", y = "", title = "") +
  theme(axis.ticks = element_blank()) +
  theme(legend.title = element_blank(), legend.position = "left") +
  scale_fill_discrete(breaks = dt$B, labels = myLabel) +
  theme(axis.text.x = element_blank())+
  geom_text(aes(y = c(4800,800) , label = mynewLabel), size = 5)

dt<-data.frame(A=c(6653,1414),B=c('p<0.05','p>0.05'))
dt = dt[order(dt$A, decreasing = TRUE),] 
myLabel = as.vector(dt$B) 
myLabel = paste(myLabel, "(",dt$A,'  ', round(dt$A / sum(dt$A) * 100, 2), "%)", sep = "")
mynewLabel=as.vector(dt$B) 
ggplot(dt, aes(x = "", y = A, fill = B)) +
  geom_bar(stat = "identity", width = 1) +
  coord_polar(theta = "y") +
  labs(x = "", y = "", title = "") +
  theme(axis.ticks = element_blank()) +
  theme(legend.title = element_blank(), legend.position = "left") +
  scale_fill_discrete(breaks = dt$B, labels = myLabel) +
  theme(axis.text.x = element_blank())+
  geom_text(aes(y = c(4800,800) , label = mynewLabel), size = 5)

dt<-data.frame(A=c(6582,159,71,1255),B=c('significant in two tests','only significant in Wilcoxon signed rank test','only significant in paired t-test','not significant in both tests'))
dt = dt[order(dt$A, decreasing = TRUE),] 
myLabel = as.vector(dt$B) 
myLabel = paste(myLabel, "(",dt$A,'  ', round(dt$A / sum(dt$A) * 100, 2), "%)", sep = "")
mynewLabel=as.vector(dt$B) 
ggplot(dt, aes(x = "", y = A, fill = B)) +
  geom_bar(stat = "identity", width = 1) +
  coord_polar(theta = "y") +
  labs(x = "", y = "", title = "") +
  theme(axis.ticks = element_blank()) +
  theme(legend.title = element_blank(), legend.position = "left") +
  scale_fill_discrete(breaks = dt$B, labels = myLabel) +
  theme(axis.text.x = element_blank())
```

######Wilcoxon signed rank test



![image-20191218183128185](/Users/quyixiang/Library/Application Support/typora-user-images/image-20191218183128185.png)

###### paired t-test

![image-20191218183233601](/Users/quyixiang/Library/Application Support/typora-user-images/image-20191218183233601.png)

###### 对比

![image-20191218183259293](/Users/quyixiang/Library/Application Support/typora-user-images/image-20191218183259293.png)

####4. Use the breast cancer dataset Table 2 and Table 3. Randomly take 20 samples of each subtypes (namely luminal A, luminal B, HER2 enriched and basal-like types) separately. ($\alpha = 0.1$)

#####(1) Use Kruskal-Wallis test to check the genes expressed differentially among the subtypes. ``Kruskal-Wallis test``

###### （a）选出需要的组的基因表达量

```R
info <- read.table("/Users/quyixiang/Desktop/生物统计/lab/lab5/Table 2.txt", sep = "\t", header = T, skip = 4)
express <- read.table("/Users/quyixiang/Desktop/生物统计/lab/lab5/Table 3.txt", header = T)

info$SAMPLE_ID <- gsub("-", ".", info$SAMPLE_ID)
info_new <- subset(info, info$SAMPLE_ID %in% names(express))

lumiA <- subset(info, info$PAM50_SUBTYPE == "Luminal A")
lumiA_id <- sample(as.character(lumiA$SAMPLE_ID), 20)
lumiA_expr <- express[c(1, which(names(express) %in% lumiA_id))]

lumiB <- subset(info, info$PAM50_SUBTYPE == "Luminal B")
lumiB_id <- sample(as.character(lumiB$SAMPLE_ID), 20)
lumiB_expr <- express[c(1, which(names(express) %in% lumiB_id))]

HER2 <- subset(info, info$PAM50_SUBTYPE == "HER2-enriched")
HER2_id <- sample(as.character(HER2$SAMPLE_ID), 20)
HER2_expr <- express[c(1, which(names(express) %in% HER2_id))]

basal <- subset(info, info$PAM50_SUBTYPE == "Basal-like")
basal_id <- sample(as.character(basal$SAMPLE_ID), 20)
basal_expr <- express[c(1, which(names(express) %in% basal_id))]
```

###### （b）对每行的基因进行k-w检验

```R
hugo_symbol <- express$Hugo_Symbol
kwtest <- data.frame(hugo_symbol)
kwtest$h_value <- 0
kwtest$p_value <- 0
subset <- c(rep("lumiA", 20),
          rep("lumiB", 20),
          rep("HER2", 20),
          rep("basal", 20))
basal_expr[1,]
for(i in 1:17267){
  expr <- c(t(lumiA_expr[i, -1]), 
            t(lumiB_expr[i, -1]), 
            t(HER2_expr[i, -1]), 
            t(basal_expr[i, -1]))
  if(anyNA(expr)){
    kwtest$h_value[i] <- NA
    kwtest$p_value[i] <- NA
  }
  else{
    temp <- kruskal.test(expr, subset)
    kwtest$h_value[i] <- temp$statistic
    kwtest$p_value[i] <- temp$p.value
  }
}
kwtest$significant <- ifelse(kwtest$p_value <= 0.1, TRUE, FALSE)
diff_gene <- subset(kwtest, kwtest$significant)
diff_gene$hugo_symbol
write.csv(diff_gene,"diff_gene.csv")
```

######最后输出的基因保存在``diff_gene.csv``中

#####(2) Do pair-wise comparisons among different subtypes for the top 10 genes with most differential expression (smallest p values). ``Wilcoxon rank sum test``

```R
kwtest_sort <- kwtest[order(kwtest$p_value), ]
top10 <- head(kwtest_sort, 10)
hugo_symbol <- top10$hugo_symbol
finalrank <- data.frame(hugo_symbol)
finalrank$lumiA_vs_lumiB <- 0
finalrank$lumiA_vs_HER2 <- 0
finalrank$lumiA_vs_basal <- 0
finalrank$lumiB_vs_HER2 <- 0
finalrank$lumiB_vs_basal <- 0
finalrank$HER2_vs_basal <- 0
for(i in finalrank$hugo_symbol){
  in_lumiA <- as.numeric(t(lumiA_expr[lumiA_expr$Hugo_Symbol == i, -1]))
  in_lumiB <- as.numeric(t(lumiB_expr[lumiB_expr$Hugo_Symbol == i, -1]))
  in_HER2 <- as.numeric(t(HER2_expr[HER2_expr$Hugo_Symbol == i, -1]))
  in_basal <- as.numeric(t(basal_expr[basal_expr$Hugo_Symbol == i, -1]))
  lumiA_vs_lumiB <- wilcox.test(in_lumiA, in_lumiB)
  lumiA_vs_HER2 <- wilcox.test(in_lumiA, in_HER2)
  lumiA_vs_basal <- wilcox.test(in_lumiA, in_basal)
  lumiB_vs_HER2 <- wilcox.test(in_lumiB, in_HER2)
  lumiB_vs_basal <- wilcox.test(in_lumiB, in_basal)
  HER2_vs_basal <- wilcox.test(in_HER2, in_basal)
  finalrank[finalrank$hugo_symbol == i, ]$lumiA_vs_lumiB <- lumiA_vs_lumiB$p.value
  finalrank[finalrank$hugo_symbol == i, ]$lumiA_vs_HER2 <- lumiA_vs_HER2$p.value
  finalrank[finalrank$hugo_symbol == i, ]$lumiA_vs_basal <- lumiA_vs_basal$p.value
  finalrank[finalrank$hugo_symbol == i, ]$lumiB_vs_HER2 <- lumiB_vs_HER2$p.value
  finalrank[finalrank$hugo_symbol == i, ]$lumiB_vs_basal <- lumiB_vs_basal$p.value
  finalrank[finalrank$hugo_symbol == i, ]$HER2_vs_basal <- HER2_vs_basal$p.value
}
finalrank
```

###### 输出

<img src="/Users/quyixiang/Library/Application Support/typora-user-images/image-20191213221012658.png" alt="image-20191213221012658" style="zoom:80%;" />

#### 5. Use the breast cancer dataset Table 2 and Table 3. It was reported that the frequency of luminal A, luminal B, HER2 enriched and basal-like subtypes in all invasive breast cancers are 35%,25%,20%,20%. (alpha = 0.05)

##### (1) Investigate the real frequency of subtypes in the breast cancer dataset, and show the results with graph.

######数据预处理

```R
library(ggplot2)

mydata<-read.table(file = "/Users/quyixiang/Desktop/生物统计/lab/lab5/Table 2.txt",skip = 4,header = T,sep = '\t')
value1<-table(mydata$PAM50_SUBTYPE)
name<-unique(mydata$PAM50_SUBTYPE)
pecent<-c(0.2,0.2,0.35,0.25)
value2<-sum(value1[1:4])*pecent
mydata1<-data.frame(NameN=rep(name[1:4],2),type=c(rep('Obsevered',4),rep('Expected',4)),Value=c(value1[1:4],value2))
```

###### 作图

```R
theme<-theme(
  legend.position = 'right',
  axis.text.y = element_text(face = 'italic'),
  axis.title.x = element_text(face = 'bold'),
  legend.title = element_text(face = 'bold'),
  title = element_text(face = 'bold'),
  panel.background = element_rect(fill='#E9E4F0')
)

ggplot()+
  geom_bar(data = mydata1,mapping = aes(x=NameN,y=Value,fill=type),stat = 'identity',width = 0.8,position = 'dodge')+
  labs(x='',y='Count',fill='Type',title = 'Frequency of Subtypes')+
  theme+
  scale_fill_manual(values = c('#307e13','#006699'))+
  coord_flip()
```

![image-20191215102102769](/Users/quyixiang/Library/Application Support/typora-user-images/image-20191215102102769.png)

##### (2) Test whether the observed frequency distribution following 35%, 25%, 20%, 20%. ``Chi-Square goodness-of-fit test``

```R
##the second question
chi<-sum((value1[1:4]-value2)**2/value2)
pvalue<-1-pchisq(chi,df = 3)
print(pvalue)
```

$p-value=1.73\times10^{-7}<0.05$，拒绝原假设，分布不是35%,25%,20%,20%.

#### 6. Use the breast cancer clinical data (Table 2). The amplification or over-expression of HER2 gene has been shown to play an important role in the development and progression of breast cancer. In recent years HER2 has become an important biomarker and target of therapy for approximately 30% of breast cancer patients. Check the signal of HEK2 can help to determine a tumor's stage. The tumor stage refers to the extent of cancer, such as how large the tumor is, and if it has spread. The higher the number, the larger the cancer tumor and the more it has spread into nearby tissues (Stage I, Stage II, and Stage III, stage IV). (alpha = 0.05)

##### (1) Test whether there is relationship between HER2_STATUS (Negative, Positive) and TUMOR_T1_CODED (T1(stage I), T_Other (other stages)) in all samples. ``Chi-Square test for independence``

###### 数据预处理

```R
t <- read.table("/Users/quyixiang/Desktop/生物统计/lab/lab5/Table 2.txt",sep="\t",header=TRUE)
x <- which((t$HER2_STATUS!="Positive" & t$HER2_STATUS != "Negative") | 
             (t$TUMOR_T1_CODED!="T_Other" & t$TUMOR_T1_CODED!="T1"))
t <- t[-x,]

chitable <- with(t,table(HER2_STATUS,TUMOR_T1_CODED))
chitable <- chitable[,-1]
chitable <- chitable[c(-1,-3),]
```

######$\chi^2$检验

```R
chisq.test(chitable)
```

###### 结果

```bash
	#Pearson's Chi-squared test with Yates' continuity correction

data:  chitable
X-squared = 4.0659, df = 1, p-value = 0.04376
# 结论：p<0.05,拒绝原假设，HER2_STATUS TUMOR_T1_CODED不独立
```

##### (2) Randomly select 30 samples only from all samples. Test the relationship between HER2_STATUS (negative, positive) and TUMOR_T1_CODED (T1, T_Other). ``Fisher’s exact test``

###### Fisher's exact test

```R
fisher.test(chitable)
```

###### 结果

```bash
#Fisher's Exact Test for Count Data

data:  chitable
p-value = 0.03484
alternative hypothesis: true odds ratio is not equal to 1
95 percent confidence interval:
 0.3246611 0.9750854
sample estimates:
odds ratio 
 0.5743605 
 # 结论：p<0.05,拒绝原假设，HER2_STATUS TUMOR_T1_CODED不独立
```

