# Load packages
library(ggplot2)
library(dplyr)
### Question (1)
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

# Analyse the one gene to see the subtype count 
group_by(gene_expressions, subtype) %>%
  summarise(
    count = n()
  )

# Normal test
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

# Variance homoscedasticity
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

# One-way anova
gene_expressions[,3][4]
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
print(paste("The differential expression genes are", paste(gene_different,collapse = ",")))

### Question (2)
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

