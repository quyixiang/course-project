### Final homework

This is the final homework, it can automatically compare the contents of the files, and then output the comparison results to four files, it can quickly output the results within 10 seconds even handling files of several hundred MB.

#### Data structure

``Suffix tree``

```C
typedef struct trieNode{
    struct trieNode *next[MAX];
}trieNode;
```

#### Algorithm

``Quicksort``

``Binary search algorithm``

#### Pattern

##### -c

**It can compare the numbers**

Use the following script and it can run automatically.

```bash
./biodiff -c -a <columnnumber1>,<columnnumber2> -b <columnnumber1>,<columnnumber2>
```

##### -n

**It can compare the strings**

Use the following script and it can run automatically.

```bash
./biodiff -n -a <columnnumber> -b <columnnumber> <filename> <filename>
```

#### Compile

```bash
gcc -o biodiff biodiff.c
```

