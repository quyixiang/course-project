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



