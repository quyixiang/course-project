#include<iostream>
#include<string>
#include<stack>

using namespace std;

//表结点
struct ArcNode {
    ArcNode * next; //Next associated edge
    int vexcode;   //Save the index of the edge and tail vertices in the vertex table
};

struct Vnode {
    string data; //Vertex name
    ArcNode * firarc; //The first edge attached to the vertex
};


class TopGraph {
private:
    int vexn; //Number of vertices in the graph
    int edge;   // The number of the edges of the graph
    int * indegree; //Penetration of each edge
    Vnode * arc; //Adjacency list
    stack<int> * SList;//List of stacks
public:
    TopGraph(int, int);
    ~TopGraph();
    //Join node
    void addNode(int, int);
    //Print adjacency list
    void print();
    //Topological sort
    void topological_sort();
};

//initialization
TopGraph::TopGraph(int vexn, int edge) {
    string course[26];
    course[0]="高等数学（A）I";
    course[1]="线性代数（B）";
    course[2]="高等数学（A）II";
    course[3]="概率统计";
    course[4]="大学物理（1）";
    course[5]="工程学导论";
    course[6]="大学物理（2）";
    course[7]="数字电子技术";
    course[8]="生物医学工程导论";
    course[9]="生物医学信号与系统（1）";
    course[10]="生物医学统计概论";
    course[11]="电子技术实验";
    course[12]="模拟电子技术";
    course[13]="生物医学信号与系统（2）";
    course[14]="生物医学图像处理（1）";
    course[15]="微机原理";
    course[16]="自动控制原理";
    course[17]="生物医学传感器";
    course[18]="生物力学";
    course[19]="生物伦理与学术道德";
    course[20]="科技实习与创新";
    course[21]="嵌入式计算机系统与实验";
    course[22]="医学仪器原理";
    course[23]="专业实习";
    course[24]="生物医学工程课程设计";
    course[25]="毕业设计";
    this->vexn = vexn;
    this->edge = edge;
    this->arc = new Vnode[this->vexn];//
    this->indegree = new int[this->vexn];
    this->SList = new stack<int> [this->vexn];
    for (int i = 0; i < this->vexn; i++) {
        this->indegree[i] = 0;
        this->arc[i].firarc = NULL;
        this->arc[i].data = course[i];
    }
}

//Free up memory space
TopGraph::~TopGraph() {
    ArcNode * p, *q;
    for (int i = 0; i < this->vexn; i++) {
        if (this->arc[i].firarc) {
            p = this->arc[i].firarc;
            while (p) {
                q = p->next;
                delete p;
                p = q;
            }
        }
    }
    delete [] this->arc;
    delete [] this->indegree;
    delete [] this->SList;
}

//Join node
void TopGraph::addNode(int start, int end) {
    //Declare a new table node
    ArcNode * temp = new ArcNode;
    temp->vexcode = end - 1;
    temp->next = NULL;
    //If the current vertex has no edge attached
    if (this->arc[start - 1].firarc == NULL) {
        this->arc[start - 1].firarc = temp;
    }
    //If the current vertex has edges attached
    else {
        ArcNode * now = this->arc[start - 1].firarc;
        //Find the last node of the linked list
        while(now->next) {
            now = now->next;
        }
        now->next = temp;
    }    
}

//Print adjacency matrix
void TopGraph::print() {
    int count = 0;
    cout << "--------------图的邻接矩阵--------------" << endl;
    //Traversing the linked list and outputting the contents of the linked list
    while (count != this->vexn) {
        //Output the nodes of the linked list
        cout << this->arc[count].data<<"： ";
        ArcNode * temp = this->arc[count].firarc;
        while (temp) {
            cout<<"<"<< this->arc[count].data<<","<< this->arc[temp->vexcode].data<<"> ";
            temp = temp->next;
        }
        cout << ";" << endl;
        ++count;
    }
}

//Sort and print
void TopGraph::topological_sort() {
    int PrintedCount=0;
    int NodeCount=0;
    int StackCount=0;
    int i;
    ArcNode * temp;

    cout << "--------------图的拓扑排序--------------" << endl;
    //Calculate the ingress of each vertex and store it in the indgree array
    for (i = 0; i != this->vexn; i++) {
        temp = this->arc[i].firarc;
        while (temp) {
            ++this->indegree[temp->vexcode];
            temp = temp->next;
        }
    }

    //Traverse the entire tree until each node is printed
    while (PrintedCount!=this->vexn){

        //Only run on first traversal
        if(PrintedCount==0){
            //Put a point with a degree of 0 on the stack
            for(i = 0; i != this->vexn; i++) {
                if (!indegree[i]) {
                    PrintedCount++;
                    cout<<this->arc[i].data<<" ";
                    SList[StackCount].push(i); 
                }
            }
            cout<<endl;
        }

        //Pop all nodes in a stack
        while (!SList[StackCount].empty()){
            i=SList[StackCount].top();
            SList[StackCount].pop();
            temp = this->arc[i].firarc;
            while (temp) {
                //If the degree of entry is reduced to 0, the next stack is entered
                if (!(--this->indegree[temp->vexcode])) {
                    cout<<arc[temp->vexcode].data<<" ";
                    PrintedCount++;
                    SList[StackCount+1].push(temp->vexcode);
                }
                temp=temp->next;
            }
        }

        //Go to the next stack
        StackCount++;
        cout<<endl;
    }
}

int main() {
    int vexn; int edge;
    TopGraph graph(26, 29);

    //Build tree
    graph.addNode(1,3);
    graph.addNode(1,4);
    graph.addNode(1,5);
    graph.addNode(2,17);
    graph.addNode(3,7);
    graph.addNode(3,19);
    graph.addNode(4,11);
    graph.addNode(5,7);
    graph.addNode(5,19);
    graph.addNode(6,9);
    graph.addNode(7,19);
    graph.addNode(8,22);
    graph.addNode(8,18);
    graph.addNode(8,12);
    graph.addNode(9,21);
    graph.addNode(9,26);
    graph.addNode(9,24);
    graph.addNode(10,14);
    graph.addNode(11,26);
    graph.addNode(12,22);
    graph.addNode(13,16);
    graph.addNode(13,23);
    graph.addNode(13,17);
    graph.addNode(14,25);
    graph.addNode(15,22);
    graph.addNode(15,25);
    graph.addNode(20,26);
    graph.addNode(23,25);
    graph.addNode(25,26);

    //Print adjacency matrix
    graph.print();
    //Topological sort
    graph.topological_sort();
    
    return 0;
}