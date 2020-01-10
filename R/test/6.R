#6.1
t <- read.table("/Users/quyixiang/Desktop/生物统计/lab/lab5/Table 2.txt",sep="\t",header=TRUE)
x <- which((t$HER2_STATUS!="Positive" & t$HER2_STATUS != "Negative") | 
             (t$TUMOR_T1_CODED!="T_Other" & t$TUMOR_T1_CODED!="T1"))
t <- t[-x,]

chitable <- with(t,table(HER2_STATUS,TUMOR_T1_CODED))
chitable <- chitable[,-1]
chitable <- chitable[c(-1,-3),]
chitable
chisq.test(chitable)
fisher.test(chitable)

