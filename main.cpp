#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <math.h>
using namespace std;
struct node
{
    node * leftChild;
    node * rightChild;
    double frequency;
    string content;
    string code;
};
int t=0;
string codes[50];
string letters[50];
vector<node> nodeArray;// Use nodeArray to record all the nodes that may be created in the whole process

node extractMin()
{
    double temp = (double) INT_MAX;
    vector<node>::iterator i1,pos;
    for(i1 = nodeArray.begin();i1!=nodeArray.end();i1++)
    {
        if(temp>(*i1).frequency)
        {
            pos = i1;
            temp = (*i1).frequency;
        }
    }
    node tempNode = (*pos);
    nodeArray.erase(pos);
    return tempNode;
}
node getHuffmanTree()
{
    while(!nodeArray.empty())
    {
        node * tempNode = new node;
        node * tempNode1 = new node;
        node * tempNode2 = new node;
        *tempNode1 = extractMin();
        *tempNode2 = extractMin();
        
        tempNode->leftChild = tempNode1;
        tempNode->rightChild = tempNode2;
        tempNode->frequency = tempNode1->frequency+tempNode2->frequency;
        nodeArray.push_back(*tempNode);
        if(nodeArray.size() == 1)//only the root node exsits
        {
            break;
        }
    }
    return nodeArray[0];
}

void BFS(node * temproot,string s)
{
    node * root1 = new node;
    root1 = temproot;
    root1->code = s;
    if(root1 == NULL)
    {
    }
    else if(root1->leftChild == NULL && root1->rightChild == NULL)
    {
        letters[t] = root1->content;
        codes[t] = root1->code;
        t++;
    }
    else
    {
        root1->leftChild->code = s.append("1");
        s.erase(s.end()-1);
        root1->rightChild->code = s.append("0");
        s.erase(s.end()-1);
        
        BFS(root1->leftChild,s.append("1"));
        s.erase(s.end()-1);
        BFS(root1->rightChild,s.append("0"));
        s.erase(s.end()-1);
    }
    //cout<<letters[0]<<" "<<codes[0]<<endl;
}
//sort letter and code array
void sortByLetter()
{
    int i,j;
    string x;
    for(i=0;i<t-1;i++)
    {
        for(j=0;j<t-i-1;j++)
        {
            if(letters[j]>letters[j+1])
            {
                x=letters[j];
                letters[j]=letters[j+1];
                letters[j+1]=x;
                
                x=codes[j];
                codes[j]=codes[j+1];
                codes[j+1]=x;
            }
        }
    }
}
void getHuffmanCode()
{
    int size,i;
    double tempDouble;
    string tempString = "";
    ifstream file;
    file.open("a4-1.txt");
    string dummyLine;
    getline(file, dummyLine);
    //cout<<dummyLine;
    //-----------------------------------------------------------String to Int
    int result = 0;
    for (i = 0; i < dummyLine.size(); i++) {
        result += (dummyLine[i] - 48) * pow(10, (dummyLine.size() - i - 1));
        //cout<<result<<endl;
    }
    //--------------------------------------------------------------
    cout<<"Frequencies are:"<<endl;
    for(i = 0;i<result;i++)
    {
        //cout<<"please input the things you want to encoded and their frequencies!"<<endl;
        node tempNode;
        file >> tempString >> tempDouble;
        cout << tempString << " " << tempDouble<<endl;
        tempNode.frequency = tempDouble;
        tempNode.content = tempString;
        tempNode.leftChild = NULL;
        tempNode.rightChild = NULL;
        nodeArray.push_back(tempNode);
    }
    cout<<endl;
    node root = getHuffmanTree();
    
    BFS(&root,"");
    sortByLetter();//sort letter and codes in alphabetical order
    //Printing Codes
    cout<<"Huffman Codes are:"<<endl;
    for(i=0;i<t;i++)
    {
        cout<<letters[i]<<" "<<codes[i]<<endl;
    }
    file.close();
}

int main()
{
    getHuffmanCode();
    string temp = "holder";
    char chars[52];
    strcpy(chars,temp.c_str());
    //--------------------Read in bit file from a4-2.txt
    ifstream file2;
    file2.open("a4-2.txt");
    string binaryLine;
    getline(file2, binaryLine);
    cout<<"Encoded Data is : \n"<<binaryLine<<endl;//Print encoded data
    //------------------------------Decode binary file
    string plain = ""; //take empty string
    int i; //to iterate
    //loop till binaryLine length is not zero
    while(binaryLine.length()>0)
    {
        //check which symbol is matching
        for(i=0;i<t;i++)//t is number of tuples from file
        {
            int n = codes[i].length(); //take length
            //if substring of length codes[i] is equals to codes[i], then append symbol to plain
            if(binaryLine.substr(0,n)==codes[i])
            {
                plain = plain + letters[i];
                binaryLine.erase (0,n); //remove that substring
                break; //break loop
            }
        }
    }
    //print decoded value
    cout<<"Decoded value: "<<plain<<endl;
    
    return 0;
}
