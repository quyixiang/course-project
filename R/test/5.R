library(ggplot2)

mydata<-read.table(file = "/Users/quyixiang/Desktop/生物统计/lab/lab5/Table 2.txt",skip = 4,header = T,sep = '\t')
value1<-table(mydata$PAM50_SUBTYPE)
name<-unique(mydata$PAM50_SUBTYPE)
pecent<-c(0.2,0.2,0.35,0.25)
value2<-sum(value1[1:4])*pecent
mydata1<-data.frame(NameN=rep(name[1:4],2),type=c(rep('Obsevered',4),rep('Expected',4)),Value=c(value1[1:4],value2))

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

##the second question
chi<-sum((value1[1:4]-value2)**2/value2)
pvalue<-1-pchisq(chi,df = 3)
print(pvalue)
