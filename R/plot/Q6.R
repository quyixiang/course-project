#第一问
breast <- read.csv("/Users/quyixiang/Desktop/生物统计/lab/lab4/brca-rsem-fpkm-tcga-t-lab.txt", header = T, sep = "\t")
breast <- subset(breast, breast$Hugo_Symbol == "PDGFD")
breast <- as.vector(t(breast[,-1]))

cervical <- read.csv("/Users/quyixiang/Desktop/生物统计/lab/lab4/cesc-rsem-fpkm-tcga-t-lab.txt", header = T, sep = "\t")
cervical <- subset(cervical, cervical$Hugo_Symbol == "PDGFD")
cervical <- as.vector(t(cervical[,-1]))

library(psych)
print(describe(breast))
print(describe(cervical))

#第二问
var.test(breast,cervical)

#第三问
t.test(breast,cervical)

#第四问
mean_brca_PDGFD<-mean(breast)
mean_cesc_PDGFD<-mean(cervical)

library("plotrix")
SE_brca_PDGFD<-std.error(breast)#计算标准
SE_cesc_PDGFD<-std.error(cervical)

bar_plot<-barplot(c(mean_brca_PDGFD,mean_cesc_PDGFD),
                  beside=T,
                  names.arg =c("brca_PDGFD","cesc_PDGFD"),
                  width =0.1,
                  col=c("#089a47","#783f04"),
                  ylim=c(0,300),
                  
)

title(ylab="Mean",
      main="Mean of PDGFD expression in the two forms of cancer",
      cex.lab=1.5, font.main= 2)

arrows(bar_plot[1], mean_brca_PDGFD - SE_brca_PDGFD,#箭头开始的横纵坐标
       bar_plot[1], mean_brca_PDGFD + SE_brca_PDGFD,
       length=0.2, 
       angle=90,
       code=3,
       lwd=2,
       col="#FF9999")

arrows(bar_plot[2], mean_cesc_PDGFD - SE_cesc_PDGFD,
       bar_plot[2], mean_cesc_PDGFD + SE_cesc_PDGFD, 
       length=0.2, 
       angle=90, 
       code=3,
       lwd=2,
       col="#FF9999")

lines(c(bar_plot[1],bar_plot[1],bar_plot[2],bar_plot[2]),
      c(mean_brca_PDGFD+SE_brca_PDGFD,250,250,mean_cesc_PDGFD+SE_cesc_PDGFD),
      lty=2,lwd=2)
text(x=(bar_plot[1]+bar_plot[2])/2, y=275, 
     label="p-value = 2.2e-16",
     cex=1.3
)
