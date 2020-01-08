#include<iostream>  
#include<string>  
using namespace std;

struct Node
{
    double weight;
    string cha;
    string code;
    //"p" is the location of the newly-created node 
    int left, right, p;
};

//Find the smallest two numbers
void Find_min(Node huffTree[], int *a, int *b, int n)  
{
    int i;
    double weight = 0;
    //Find the smallest number
    for (i = 0; i <n; i++)
    {
        //Define whether or not the node has been selected
        if (huffTree[i].p != -1)
            continue;
        else
        {
            if (weight == 0)
            {
                weight = huffTree[i].weight;
                *a = i;
            }
            else
            {
                if (huffTree[i].weight < weight)
                {
                    weight = huffTree[i].weight;
                    *a = i;
                }
            }
        }
    }
    //Find the second smallest number
    weight = 0;
    for (i = 0; i < n; i++)
    {
        //Exclude the number has been selected
        if (huffTree[i].p != -1 || (i == *a))
            continue;
        else
        {
            if (weight == 0)
            {
                weight = huffTree[i].weight;
                *b = i;
            }
            else
            {
                if (huffTree[i].weight < weight)
                {
                    weight = huffTree[i].weight;
                    *b = i;
                }
            }
        }
    }
    int temp;
    if (huffTree[*a].left < huffTree[*b].left)
    {
        temp = *a;
        *a = *b;
        *b = temp;
    }
}

//Build a new huffman tree according to "cha"
void Build_Tree(Node huffTree[], double w[], string cha[], int n)
{
    //Initiation
    for (int i = 0; i < 2 * n - 1; i++)
    {
        huffTree[i].p = -1;    
        huffTree[i].left = -1;    
        huffTree[i].right = -1;  
        huffTree[i].code = "";
    }
    for (int i = 0; i < n; i++)
    {
        huffTree[i].weight = w[i];  
        huffTree[i].cha = cha[i];     
    }
    for (int k = n; k < 2 * n - 1; k++)
    {
        int i1 = 0;
        int i2 = 0;
        //Combine i1 and i2 to k
        Find_min(huffTree, &i1, &i2, k);
        huffTree[i1].p = k;   
        huffTree[i2].p = k;
        //The weight of the new node is the sum of the weight of the original two nodes
        huffTree[k].weight = huffTree[i1].weight + huffTree[i2].weight;
        huffTree[k].left = i1;
        huffTree[k].right = i2;
    }
}

//Find all of the huffman code
void Get_Code(Node huffTree[], int n)
{
    int i, j, next;
    //Save the information of the huffman tree in "s"
    string str = "";
    for (i = 0; i < n; i++)
    {
        str = "";
        j = i;
        while (huffTree[j].p != -1)
        {
            next = huffTree[j].p;

            if (j == huffTree[next].left)
            {
                str = str + "0";
            }
            else               
            {
                str = str + "1";
            }
            j = huffTree[j].p; 
        }
        for (int m = str.size() - 1; m >= 0; m--)    
        {
            //Save the code
            huffTree[i].code = huffTree[i].code + str[m];
        }

    }
}

//Get the Huffman code accoding to the input word
string Huff_Getcode(Node hufftree[], int n,string s)
{
    cout << "After Huffman encoding, the output is:"<<endl;
    string temp="",str="";
    for (int i = 0; i < s.size();i++)
    {
        for (int j = 0; j < n; j++)       
        {   
            temp = s[i];
            if (temp==hufftree[j].cha){
                str=str+hufftree[j].code;
                break;
            }
            if (j==n-1){
                str = "Error!";
                //If there is something wrong in the input, "i" will become big enough to stop the circulation
                i = s.size()-1;
            }    
        }
    }
    return str;
}

//Get the word according to the input code
string Huff_Getword(Node huffTree[], int n,string s)
{
    cout << "After Huffman decoding, the output is:"<<endl;
    string temp = "",str="";
    for (int i = 0; i < s.size(); i++)
    {
        temp = temp + s[i];
        for (int j = 0; j < n; j++)
        {    
            if (temp == huffTree[j].code)
            {
                str=str+ huffTree[j].cha;
                temp = "";
                break;
            }    
            else if (i == s.size()-1&&j==n-1&&temp!="")
            {
                str= "Error!";
                i = s.size()-1;
            }
        }
    }
    return str;
}

int main()
{
    //Encode
    const int n=26;
    Node huffTree[2 * n];
    string str[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
    //The frequency of the English words
    double w[] = {8.19,1.47,3.83,3.91,12.25,2.26,1.71,4.57,7.10,0.14,0.41,3.77,3.34,7.06,7.26,2.89,0.09,6.85,6.36,9.41,2.58,1.09,1.59,0.21,1.58,0.08};
    //Build a huffman tree
    Build_Tree(huffTree, w, str, n);
    //Save the huffman code of each character
    Get_Code(huffTree, n);
    //Decode
    string s;
    cout << "Please input a Huffman code:"<<endl;
    cin >> s;
    //Using the code to get the word
    cout << Huff_Getword(huffTree, n, s)<< endl;
    cout << "Please input a string to convert:"<<endl;
    cin >> s;
    //Using the word to get the code
    cout << Huff_Getcode(huffTree, n, s)<< endl;
    return 0;
}