#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLEN 1024
#define buffer_size 8388608
#define MAX 126


//define a struct that can store the information(-c pattern)
struct node_c{
    int start;
    int end;
    int mark;//use it to determine while file to output, mark=1 overlap, mark=0, not overlap
    int length;
    char *line;//the content of the line
};

//define a struct that can store the information(-n pattern)
struct node_n{
    char *name;//the name
    char *line;//the content of the line
};

//define a trie to save the words
typedef struct trieNode{
    struct trieNode *next[MAX];
}trieNode;


//declear the function to be used
int gets_c(struct node_c *node[], FILE *fp, int s, int e);
struct node_c *tallocc(void);
void quicksort(struct node_c *a[], int left, int right);
int max(struct node_c *node[], int m);
int search(int aim, struct node_c *s[],int n);
void fputs_c(struct node_c *a[], struct node_c *b[], int m, int n, int maxlength, FILE *f1, FILE *f2);
void freeMemory_c(struct node_c *line[], int num);
int gets_n(struct node_n *p[], FILE *f, int n);
struct node_n *tallocn(void);
trieNode *createTrieNode(void);
void insert(trieNode *ptrRoot, struct node_n *a);
int count(struct node_n *a, trieNode *root, FILE *f1, FILE *f2);
void delTrie(trieNode *root);
void fputs_n(struct node_n *a[], struct node_n *b[], int m, int n, FILE *f1, FILE *f2);
void freeMemory_n(struct node_n *line[], int num);

//define the global variable
FILE *fp1, *fp2, *fpa, *fpb, *fpa_b, *fpb_a;
struct node_c *line_c1[buffer_size];
struct node_c *line_c2[buffer_size];
struct node_n *line_n1[buffer_size];
struct node_n *line_n2[buffer_size];

//the main function
int main(int argc, char *argv[]){
    //declear the variable used in the main function
    int lineNumA, lineNumB;
    int maxLengthA,maxLengthB;

    
    //test whether or not the file is useable
    if(argc!=8){
        printf("The keys you input is wrong.\nIf you want to use -c pattern, you should input something like \"./biodiff -c -a <linenumber>,<linenumber> -b <linenumber>,<linenumber> <filename> <filename>\"\nIf you want to use -n pattern, you should input something like \"./biodiff -n -a <linenumber> -b <linenumber> <filename> <filename>\"\n");
        exit(0);
    }
    else{

        //open the input file
        fp1=fopen(argv[6],"r");
        fp2=fopen(argv[7],"r");

        //test whether or not the file can be open
        if(fp1==NULL){
            printf("can't open %s\n",argv[6]);
            exit(0);
        }

        else if(fp2==NULL){
            printf("can't open %s\n",argv[7]);
            exit(0);
        }

        else{

            //open the file to be written in
            fpa=fopen("A&A_B.txt","w");
            fpb=fopen("B&A_B.txt","w");
            fpa_b=fopen("A-B.txt","w");
            fpb_a=fopen("B-A.txt","w");
        
            //use the -c pattern
            if ((argc == 8) && (strcmp(argv[1], "-c") == 0) && (argv[2][0] == '-') && (argv[3][1] == ',') && (argv[4][0] == '-') && (argv[5][1] == ',')){

                //get the input number in order to decided which column to analyze
                int sA = (argv[3][0] - '0');
                int eA = (argv[3][2] - '0');
                int sB = (argv[5][0] - '0');
                int eB = (argv[5][2] - '0');
            
                //Read the number of A rows, and save the contents of file A into the structure
                lineNumA=gets_c(line_c1, fp1, sA, eA);
                lineNumB=gets_c(line_c2, fp2, sB, eB);

                //if the line number is 0, print the error
                if(lineNumA==0||lineNumB==0){
                    printf("At least one of the input files is empty, please check.");
                    exit(0);
                }
            
                //quicksort according to the end of the struct
                quicksort(line_c1, 0, lineNumA-1);
                quicksort(line_c2, 0, lineNumB-1);

                //get the max length of the files
                maxLengthA=max(line_c1,lineNumA);
                maxLengthB=max(line_c2,lineNumB);

                //put the word into the target files
                fputs_c(line_c1, line_c2, lineNumA, lineNumB, maxLengthB, fpa_b, fpa);
                fputs_c(line_c2, line_c1, lineNumB, lineNumA, maxLengthA, fpb_a, fpb);
             
                //free the memory
                freeMemory_c(line_c1, lineNumA);
                freeMemory_c(line_c2, lineNumB);
            }
        
        
            //use the -n pattern
            else if ((argc == 8) && (argv[1][0] == '-') && (argv[1][1] == 'n') && (argv[2][0] == '-') && (argv[4][0] == '-')){
            
                //get the input number
                int nA=atoi(argv[3]);
                int nB=atoi(argv[5]);
            
                //read the number of the line of the files and save the content into the structure
                lineNumA=gets_n(line_n1, fp1, nA);
                lineNumB=gets_n(line_n2, fp2, nB);

                //if the line number is 0, print the error
                if(lineNumA==0||lineNumB==0){
                    printf("At least one of the input files is empty, please check.\n");
                    exit(0);
                }
            
                //output the lines into two files
                fputs_n(line_n2, line_n1, lineNumB, lineNumA, fpa_b, fpa);
                fputs_n(line_n1, line_n2, lineNumA, lineNumB, fpb_a, fpb);
            
                //free the memory
                freeMemory_n(line_n1, lineNumA);
                freeMemory_n(line_n2, lineNumB);
            }
            
        
            //the wrong input
            else{
                printf("There may be some errors in your input.\nIf you want to use -c pattern, you should input something like \"./biodiff -c -a <linenumber>,<linenumber> -b <linenumber>,<linenumber> <filename> <filename>\"\nIf you want to use -n pattern, you should input something like \"./biodiff -n -a <linenumber> -b <linenumber> <filename> <filename>\"\n");
            }

            //close the files
            fclose(fpa_b);
            fclose(fpb_a);
            fclose(fpa);
            fclose(fpb);
        }

        fclose(fp1);
        fclose(fp2);

        //Congratulations
        printf("The new files have been successfully made.\nENJOY.\n");
    }
}

//gets_c:define the number of rows, and save the content into struct
int gets_c(struct node_c *node[], FILE *fp, int s, int e){
    char buffer[MAXLEN];
    int i,j=0;
    char *o,*l[20];
    while(fgets(buffer,MAXLEN,fp)!=NULL){
        
        //ignore the blank lines
        if(sizeof(buffer)>1){
            //save the information into the struct
            node[j]=tallocc();
            o=(char *)malloc(strlen(buffer)+1);
            node[j]->line=strcpy(o,buffer);
            node[j]->mark=0;//initiate mark
            l[0] = strtok(buffer, "\t"" ");//split characters by tab
            for (i = 1; (l[i] = strtok(NULL, "\t"" ")) != NULL; i++)
                ;
            node[j]->start=atoi(l[s]);
            node[j]->end=atoi(l[e]);
            node[j]->length=node[j]->end - node[j]->start;
            
            j++;
            if(j%10000==0){
                printf("read %d lines\n",j);
            }
        }
    }
    
    //get the number of the lines that have content
    return j;
}

//define a function to allocate memory
struct node_c *tallocc(void){
    return (struct node_c *)malloc(sizeof(struct node_c));
}

//quicksort, we can use it to sort the end of the gene in the right order
void quicksort(struct node_c *a[], int left, int right){
    int i,j;
    struct node_c *t,*temp;

    if(left>right)
        return;

    temp=a[left];
    i=left;
    j=right;

    while(i!=j){
        while(a[j]->end>=temp->end && i<j)
            j--;
        while(a[i]->end<=temp->end && i<j)
            i++;
        if(i<j){
            t=a[i];
            a[i]=a[j];
            a[j]=t;
        }
    }

    a[left]=a[i];
    a[i]=temp;

    //recursion
    quicksort(a,left,i-1);
    quicksort(a,i+1,right);
}

//max:get the maximum coordinate length in the structure array pointer
int max(struct node_c *node[], int m){
  int i;
  int maxlength = 0;
  for (i=0; i<m; i++){
    if (maxlength < node[i]->length){
        maxlength = node[i]->length;
    }
  }
  return maxlength;
}

//search:dichotomy finds the end coordinate that are greater than or equal to the minimum target value
int search(int aim, struct node_c *s[],int n){
    int mid;
    int left = 0;
    int right = n-1;
    while (left < right - 1){
        mid = (left + right)/2;
        if(s[mid]->end >= aim){
            right = mid;
        }
        
        else{
            left = mid;
        }
        
        if (s[left]->end >= aim){
            right = left;
        }
    }
    return right;
}

//see whether the numbers are overlapped or not and fput them in the files
void fputs_c(struct node_c *a[], struct node_c *b[], int m, int n, int maxlength, FILE *f1, FILE *f2){
    int i,j,h;
    
    for(i=0;i<m;i++){
        if(a[i]->mark==1){
            fputs(a[i]->line, f2);
        }
        //if the max of b is smaller than a[i], then it is definately not overlapped
        else if(b[n-1]->end<a[i]->start){
            fputs(a[i]->line,f1);
        }
        else{
            h=search(a[i]->start, b ,n);//find b where the end of it is greater than that start of a
            for(j=h;j<n;j++){
                if(b[j]->start<=a[i]->end){
                    fputs(a[i]->line, f2);
                    a[i]->mark=1;
                    b[j]->mark=1;
                    break;
                }
                //if the smallest end number of b minus the longest length of the gene is still larger than the end of a, then it is definatelly not overlapped
                if (((b[j]->end)-maxlength) > a[i]->end) {
                    break;
                }
            }
            //after the loop, the mark is still 0, it means that it is not overlapped
            if(a[i]->mark==0){
                fputs(a[i]->line, f1);
            }
        }
        //record the lines been put in order to test the speed
        if(i%10000==0){
            printf("put %d lines\n",i);
        }
    }
}

//free the memory the function used
void freeMemory_c(struct node_c *line[], int num){
    int i;
    for(i=0;i<num;i++){
        free(line[i]);
        line[i]=NULL;
    }
}

//read the number of lines, and save the content of files into the struct
int gets_n(struct node_n *node[], FILE *f, int n){
    char buffer[MAXLEN];
    int i,j=0;
    char *l[20];
    char *temp1,*temp2;
    while(fgets(buffer, MAXLEN, f)!=NULL){
        if(strlen(buffer)!=1){
            node[j]=tallocn();
            temp1=(char *)malloc(strlen(buffer)+1);
            node[j]->line=strcpy(temp1,buffer);
            //l is an array that store the words of the line
            l[0] = strtok(buffer, "\t"";"" ""\"""\n");
            for (i = 1; (l[i] = strtok(NULL, "\t"" "";""\"""\n")) != NULL; i++)
            ;
            temp2 = (char *) malloc(strlen(l[n]) +1);
            node[j]->name = strcpy(temp2,l[n]) ;
            j++;
            //record the lines been read in order to test the speed
            if(j%10000==0){
                printf("read %d lines\n",j);
            }
        }
    }
    //return the numbers of the line
    return j;
}

//define a function to allocate memroy
struct node_n *tallocn(void){
    return(struct node_n *)malloc(sizeof(struct node_n));
}

//create a new node and initiate it
trieNode *createTrieNode(void){
    //create
    trieNode *treeNode=NULL;
    treeNode=(trieNode *)malloc(sizeof(trieNode));
    //initiate
    memset(treeNode->next, 0x00, sizeof(treeNode->next));
    return treeNode;
}

//insert the name into the tree
void insert(trieNode *ptrRoot, struct node_n *node){
    int i;
    char *ascii;
    trieNode *pointer=ptrRoot;

    ascii=(char *)malloc(strlen(node->name)+1); 
    strcpy(ascii,node->name);
    while(*ascii!='\0'){
        //if the character has been saved, the pointer should point to the next node
        if(pointer->next[*ascii]!=NULL){
            pointer=pointer->next[*ascii];
            ascii++;
            continue;
        }
        //if the character hasn't been saved, we should make a new node to save it
        pointer->next[*ascii]=createTrieNode();
        pointer=pointer->next[*ascii];
        ascii++;
    }
}

//see whether the name is in the tree or not and fput them in the files
int count(struct node_n *node, trieNode *root, FILE *f1, FILE *f2){
    trieNode *pointer=root;
    char *tempLine, *ascii;
    int i,index;
    tempLine=(char *)malloc(strlen(node->name)+1);
    ascii=strcpy(tempLine, node->name);

    while(*ascii!='\0'){
        
        //not in the tree
        if(pointer->next[*ascii] == NULL){
            fputs(node->line,f1);
            break;
        }

        //in the tree, the pointer points to the next node
        else{
            pointer=pointer->next[*ascii];
            ascii++;
        }
    }

    //if *ascii == 0, it means that the word is in the tree
    if(*ascii=='\0'){
        fputs(node->line, f2);
    }
    free(tempLine);
    return 0;
}

//delete the tree
void delTrie(trieNode *root){
    int i;
    
    for(i=0;i<MAX;i++){
        if(root->next[i]!=NULL){
            delTrie(root->next[i]);
        }
    }
    
    free(root);
    root=NULL;
}

//diff based on name
void fputs_n(struct node_n *a[], struct node_n *b[], int m, int n, FILE *f1, FILE *f2){
    int j;
    trieNode *root;
    root=createTrieNode();
    
    //save the information of a in the tree
    for(j=0;j<m;j++){
        insert(root, a[j]);
        //record the lines been inserted in order to test the speed
        if(j%10000==0){
            printf("insert %d lines\n",j);
        }
    }
    printf("\n----------------------------\ninsert successfully!!!\n");
    //compare the words of b with the words in a
    for(j=0;j<n;j++){
        count(b[j],root,f1,f2);
        //record the lines been count in order to test the speed
        if(j%10000==0){
            printf("count %d lines\n",j);
        }
    }
    printf("\n----------------------------\ncount successfully!!!\n");
    delTrie(root);
}

//free the memory have been used
void freeMemory_n(struct node_n *line[], int num){
    int i;
    for(i=0;i<num;i++){
        free(line[i]);
        line[i]=NULL;
    }
}