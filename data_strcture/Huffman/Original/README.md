##Hoffman encode and decode

####Purpose

This is a algorithm using stack, and it can solve the following question.

```
Write a program that solves Huffmann encoding and decoding.
Encoding part: the input is a string and the output is the Huffmann encoding of each character.
Decoding part: The input is a 01 sequence and decoded into a string.
```

#### Ideas

According to the principle of Huffman coding, a Huffman tree composed of 26 English letters is first obtained according to the weight of different letters, and a decoding method is obtained according to the coding of each node of the Huffman tree.

#### Code block

##### initialization

```c++
void Build_Tree(Node huffTree[], double w[], string cha[], int n)
```

##### Get the encoding of each leaf node of the Huffman tree

```c++
void Get_Code(Node huffTree[], int n)
```

##### Huffman encoding based on words

```C++
string Huff_Getcode(Node hufftree[], int n,string s)
```

##### Return words based on Huffman coding

```C++
string Huff_Getword(Node huffTree[], int n,string s)
```

#### Instructions

##### Compile

```bash
g++ -o Huffman Huffman.cpp
```

##### Example

```bash
g++ -o Huffman Huffman.cpp
./Huffman 
Please input a Huffman code:
01110110010010011011101010001111011100110110
After Huffman decoding, the output is:
inspiration
Please input a string to convert:
inspiration
After Huffman encoding, the output is:
01110110010010011011101010001111011100110110
```

------

###### Note: According to tests, the Huffman codes of the 26 English letters in this example are as follows.

```bash
The Huffman code of "a" is:0001
The Huffman code of "b" is:100010
The Huffman code of "c" is:00000
The Huffman code of "d" is:00001
The Huffman code of "e" is:101
The Huffman code of "f" is:11001
The Huffman code of "g" is:100001
The Huffman code of "h" is:1101
The Huffman code of "i" is:0111
The Huffman code of "j" is:110000001
The Huffman code of "k" is:1100001
The Huffman code of "l" is:00101
The Huffman code of "m" is:00100
The Huffman code of "n" is:0110
The Huffman code of "o" is:0011
The Huffman code of "p" is:10011
The Huffman code of "q" is:1100000001
The Huffman code of "r" is:0101
The Huffman code of "s" is:0100
The Huffman code of "t" is:111
The Huffman code of "u" is:10010
The Huffman code of "v" is:110001
The Huffman code of "w" is:100000
The Huffman code of "x" is:11000001
The Huffman code of "y" is:100011
The Huffman code of "z" is:1100000000
```

