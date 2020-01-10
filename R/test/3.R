#(1)
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

#(2)
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
genes_pvalue_1
#(3)
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
#画图
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

