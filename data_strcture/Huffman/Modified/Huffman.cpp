#include <iostream>
#include <vector>

using namespace std;

//Define a pair of data types to facilitate dictionary construction
typedef pair<string, string> pair_data;

//Define linked list nodes
struct node{
    node* next;
    node* last;
    pair_data data;
};

//Huffman
struct HuffmanTreeNode {
	char c;  //Corresponding encoded character
	HuffmanTreeNode *left;
	HuffmanTreeNode *right;
	HuffmanTreeNode(char x) : c(x), left(NULL), right(NULL) {}

};

//Record characters and frequencies to correspondence
struct char_rate{
	char c; //Coded character
	int n; //Frequency
	HuffmanTreeNode *node; //Corresponding tree node, used when coding

	char_rate(char _c, int _n) :c(_c), n(_n) {}
};

//Define a dictionary to facilitate the storage of the correspondence between leaf node characters and their encodings
class dictionary_creat{
	private:
		node* head;
		node* tail;
		int length;//The length of the list
	public:
		dictionary_creat();
		~dictionary_creat();

		//Insert a pair of data
		void insert(pair_data & element);
		//Find corresponding letters by code
		pair_data find_code(string code);
		//Find the corresponding code by letter
		pair_data find(string name);
};

//Initiaition
dictionary_creat::dictionary_creat(){
    head = new node;
    tail = new node;
    head->next = tail;
    head->last = nullptr;
    tail->last = head;
    tail->next = nullptr;
    length = 0;
}

dictionary_creat::~dictionary_creat(){}

//Insert a dictionary element
void dictionary_creat::insert(pair_data &element){
    tail->data = make_pair(element.first, element.second);
    tail->next = new node;
    node* p = tail;
    tail = p->next;
    tail->last = p;
    tail->next = nullptr;
    length++;
}

//Find the corresponding letter according to the encoding
pair_data dictionary_creat::find_code(string code)
{
    node* p = head->next;
	pair_data res,fal;
    while(p!=tail)
    {
        if(!(p->data).second.compare(code))
        {
			res=p->data;
			return res;
        }
        p = p->next;
    }
    fal=make_pair("0","0");
    return fal;
}

//Find the corresponding code by letter
pair_data dictionary_creat::find(const string name)
{
    node* p = head->next;
	pair_data res,fal;
    while(p!=tail)
    {
        if(!(p->data).first.compare(name))
        {
			res=p->data;
			return res;
        }
        p = p->next;
    }
	fal=make_pair("0","0");
    return fal;
}

//Use a vector to implement the heap operation in order to find find the smallest element
void insertHuffmanTreeValue(vector<char_rate> & HuffmanTree, char_rate hc){
	HuffmanTree.push_back(hc);

	int len = HuffmanTree.size();
	auto childIdx = len - 1;
	auto parentIdx = (childIdx - 1) / 2;

	//Sort using the smallest heap
	while (parentIdx >= 0){
		//If the current child node is larger than the parent node, then exchange data
		if (HuffmanTree[parentIdx].n > HuffmanTree[childIdx].n){
			auto t = HuffmanTree[parentIdx];
			HuffmanTree[parentIdx] = HuffmanTree[childIdx];
			HuffmanTree[childIdx] = t;
			childIdx = parentIdx;
			parentIdx = (childIdx - 1) / 2;
		}
		else{
			break;
		}
	}
}

//Find the current smallest node, return and delete
char_rate find_min(vector<char_rate> & HuffmanTree){
	if (HuffmanTree.empty()){
		return char_rate('0',-1);
	}
	//Returns the first value of the smallest heap
	auto res = HuffmanTree[0];
	//Delete the first element of the smallest heap
	HuffmanTree[0] = HuffmanTree.back();
	HuffmanTree.pop_back();

	int len = HuffmanTree.size();
	auto parentIdx = 0;
	auto childIdx = 1;

	//Reorder the smallest heap
	while (childIdx <= len - 1){
		if ((childIdx != len-1) && (HuffmanTree[childIdx].n > HuffmanTree[childIdx+1].n)){
			childIdx = childIdx + 1;
		}
		//If the parent node is larger than the child node, then exchange data
		if (HuffmanTree[childIdx].n < HuffmanTree[parentIdx].n){
			auto t = HuffmanTree[parentIdx];
			HuffmanTree[parentIdx] = HuffmanTree[childIdx];
			HuffmanTree[childIdx] = t;
			parentIdx = childIdx;
			childIdx = 2 * childIdx + 1;
		}
		else{
			break;
		}
	}
	return res;
}

//Huffman tree construction
HuffmanTreeNode * Huffman_Encoding(vector<char_rate> & HuffmanTree){
	while (!HuffmanTree.empty()){
		if (HuffmanTree.size() == 1){
			return HuffmanTree[0].node;
		}
		else{
			//Find the smallest and second smallest nodes first
			auto x = find_min(HuffmanTree);
			auto y = find_min(HuffmanTree);
			//Place the smallest node in the left subtree
			HuffmanTreeNode * left;
			//Mix the node
			if (x.c != 0){
				left = new HuffmanTreeNode(x.c);
			}
			else{
				left = x.node;
			}
			//Put the second smallest node into the right subtree
			HuffmanTreeNode * right;
			if (y.c != 0){
				right = new HuffmanTreeNode(y.c);
			}
			else{
				right = y.node;
			}
			HuffmanTreeNode * z = new HuffmanTreeNode(0);
			z->left = left;
			z->right = right;
			//Set the frequency of the new node as the sum of the frequencies of the left and right nodes
			char_rate hc(0, x.n + y.n);
			hc.node = z;
			//Insert new node into Huffman tree
			insertHuffmanTreeValue(HuffmanTree, hc);
		}
	}
	return HuffmanTree[0].node;
}

//Recursively traverse the subtree, prefix indicates the current prefix encoding, and when recursive to the leaf node, insert the leaf node into the dictionary
void outputHuffman(HuffmanTreeNode * root, string prefix, dictionary_creat &datas){
	if (root->left == NULL && root->right == NULL){
		pair_data data;
		data=make_pair(root->c,prefix);
		datas.insert(data);
		std::cout << root->c << " " << prefix.c_str() << std::endl;
	}
	else{
		outputHuffman(root->left, prefix + "0",datas);
		outputHuffman(root->right, prefix + "1",datas);
	}
}

//Start traversal
void outputHuffmanTreeBinary(HuffmanTreeNode * root,dictionary_creat &datas){
	if (root != NULL){
		//Output the Huffman coding of the left subtree
		outputHuffman(root->left, "0",datas);
		//Output the Huffman coding of the right subtree
		outputHuffman(root->right, "1",datas);
	}
}

//Find the corresponding letter string according to a string of Huffman codes
string Getword(dictionary_creat datas, string s){
	string temp = "",str="";
    for (int i = 0; i < s.size(); i++)
    {
        temp = temp + s[i];
		string temp_word;
		temp_word=datas.find_code(temp).first;
 
		if (temp_word!="0")
		{
			str=str+temp_word;
			temp = "";
		}    
		else if (i == s.size()-1&&temp!="")
		{
			str= "Error!";
			i = s.size()-1;
		}
	}
	return str;
}

//Find the corresponding Huffman code according to a string of letters
string Getcode(dictionary_creat datas, string s)
{
    string temp="",str="";
    for (int i = 0; i < s.size();i++)
    {

		temp = s[i];
		string temp_code;
		temp_code=datas.find(temp).second;
		if (temp_code!="0"){
			str=str+temp_code;
		}
		else{
			str = "Error!";
			i = s.size()-1;
        }
    }
    return str;
}

int main(){
	vector<char_rate> HuffmanTree;
	//Find the relationship between letters and their frequency
	vector<pair<char, float>> ci = {
		{'A', 8.19},
		{'B', 1.47},
		{'C', 3.83},
		{'D', 3.91},
		{'E', 12.25},
		{'F', 2.26},
        {'G', 1.71},
		{'H', 4.57},
		{'I', 7.10},
		{'J', 0.14},
		{'K', 0.41},
		{'L', 3.77},
		{'M', 3.34},
		{'N', 7.06},
		{'O', 7.26},
		{'P', 2.89},
		{'Q', 0.09},
		{'R', 6.85},
		{'S', 6.36},
		{'T', 9.41},
		{'U', 2.58},
		{'V', 1.09},
		{'W', 1.59},
		{'X', 0.21},
		{'Y', 1.58},
		{'Z', 0.08},
	};
	
	//Insert the above frequencies into the Huffman tree
	for (auto x : ci){
		char_rate hc(x.first, x.second);
		insertHuffmanTreeValue(HuffmanTree, hc);
	}
	//Output the Huffman coding for A-Z
	cout<<"------------------Huffman prefix code------------------"<<endl;
	auto root = Huffman_Encoding(HuffmanTree);
	dictionary_creat datas;
	outputHuffmanTreeBinary(root,datas);
	string s;
	//Find the corresponding letter string according to Huffman coding
	cout<<"---------------------Huffman decode---------------------"<<endl;
    cout << "Please input a Huffman code:"<<endl;
    cin >> s;
    cout << Getword(datas, s)<< endl;
	//Find the Huffman code from a string of letters
	cout<<"---------------------Huffman encode---------------------"<<endl;
    cout << "Please input a string to convert:"<<endl;
    cin >> s;
    cout << Getcode(datas, s)<< endl;

	return 0;
}

