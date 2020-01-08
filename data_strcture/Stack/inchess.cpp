#include <iostream>

using namespace std;

#define MaxStackSize 10000

struct Locate
{
    int x;
    int y;
};

struct Node
{
    int x;
    int y;
    int Order;
};
//int Chess[M+4][N+4];
Locate Move[9]={{0,0},{1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1},{-2,1},{-1,2}};

class Stacktype{
    public:
        Stacktype(int m);
        ~Stacktype();
        bool IsEmpty() {return Top==0;};
        bool IsFull() {return Top==Size-1;};
        void Push(Node Item);
        Node Pop();
        void PrintPath();
        int TopValue() {return Top;};
        Node DataValue(int i) {return Data[i];};
    private:
        int Size;
        int Top; 
        Node Data[MaxStackSize];
};
Stacktype::Stacktype(int m){
    Size=m*m;
    for (int i=0;i<Size;i++){
        Data[i].x=0;
        Data[i].y=0;
    }
    Top=0;
};

Stacktype::~Stacktype(){}

void Stacktype::PrintPath(){
    cout<<"This is the path:"<<endl;
    for (int i=1;i<=Top;i++) cout<<Data[i].x-1<<","<<Data[i].y-1<<"-->";
};

void Stacktype::Push(Node Item){
    if (IsFull()) cout<<"The stack is full."<<endl;
    else{
        Top+=1;
        Data[Top]=Item;
    }
};

Node Stacktype::Pop(){
    if (IsEmpty()) cout<<"The stack is empty."<<endl;
    else return Data[Top--];
};

//This function is used to verify whether the previous stack contains the current point, and returns false if there is.
bool Test(Node test,Stacktype Stack,int Top){
    int flag=0;
    for (int i=0;i<=Top;i++){
        if(test.x==Stack.DataValue(i).x&&test.y==Stack.DataValue(i).y){
            flag+=1;
        }
    }
    if (flag==0) return true;
    else return false;
}

int main(int argc,char *argv[]){
    int m;
    cout<<"Please enter the size of the board."<<endl;
    cin>>m;
    Stacktype Stack(m);
    int startx,starty;
    int Nextx,Nexty;
    Node CurrentPosition;
    bool Full=false;

    //Initialization
    int Chess[m+4][m+4];
    for(int i=0;i<m+4;i++){
        for(int j=0;j<m+4;j++){
            Chess[i][j]=1;
        }
    }
    for(int i=2;i<m+2;i++){
        for(int j=2;j<m+2;j++){
            Chess[i][j]=0;
        }
    }

    //Push the indormation of the original position into the stack.
    cout<<"Please input the start position."<<endl;
    cin>>startx>>starty;
    CurrentPosition.x=startx+1;
    CurrentPosition.y=starty+1;
    CurrentPosition.Order=0;
    Stack.Push(CurrentPosition);

    //If the stack is empty, there is no path, if the stack is full, the path has been found
    while((!Stack.IsEmpty())&&(!Stack.IsFull())){
        CurrentPosition=Stack.Pop();
        cout<<"The number of elements in the current stack is:"<<Stack.TopValue()<<endl;
        CurrentPosition.Order+=1;
        
        //If it tried in all 8 directions and failed, you should push this point out of the stack
        while ((CurrentPosition.Order<=8)&&(!Full)){
            Nextx=CurrentPosition.x+Move[CurrentPosition.Order].x;
            Nexty=CurrentPosition.y+Move[CurrentPosition.Order].y;

            if (Stack.IsFull())
            {
                Stack.PrintPath();
                cout<<CurrentPosition.x-1<<","<<CurrentPosition.y-1<<endl;
                Full=true;
            }

            else{
                Node test;
                test.x=Nextx;
                test.y=Nexty;
                //When the next point is not outside the fence and is not on the stack, you can put it on the stack
                if ((Chess[Nextx][Nexty]==0)&&Test(test,Stack,Stack.TopValue())){
                    Stack.Push(CurrentPosition);
                    cout<<"The number of elements in the current stack is "<<Stack.TopValue()<<endl;
                    CurrentPosition.x=Nextx;
                    CurrentPosition.y=Nexty;
                    CurrentPosition.Order=0;
                }
                CurrentPosition.Order+=1;
            } 
        }
    }

    //If the stack is still empty after the loop, there is no path
    if (Stack.IsEmpty()){
        cout<<"There is no path."<<endl;
    }
}
