#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;
struct headNode
{
    headNode * left_Child;
    headNode * right_Child;
    float freq;
    string content;
    string code;
};


string codes[52];
string letters[52];
vector<headNode> treeArr;

headNode getLower()
{
    float holder = 500000;
    vector<headNode>::iterator comparator,position;
    for(comparator = treeArr.begin();comparator!=treeArr.end();comparator++)
    {
        if(holder > (*comparator).freq)
        {
            position = comparator;
            holder = (*comparator).freq;
        }
    }
    headNode holdingNode = (*position);
    treeArr.erase(position);
    return holdingNode;
}

headNode buildHTree()
{
    while(!treeArr.empty())
    {
        int sizeH = 1;
        headNode * holdingNode = new headNode;
        headNode * holdingNodeDuo = new headNode;
        headNode * holdingNodeTrio = new headNode;

        *holdingNodeDuo = getLower();
        *holdingNodeTrio = getLower();

        holdingNode->left_Child = holdingNodeDuo;
        holdingNode->right_Child = holdingNodeTrio;
        holdingNode->freq = (holdingNodeDuo->freq) + (holdingNodeTrio->freq);

        treeArr.push_back(*holdingNode);
        if(treeArr.size() == sizeH)
        {
            break;
        }
    }
    return treeArr[0];
}

int orderOfLetters=0;
void binaryBuilder(headNode * hold,string stringArr)
{
    headNode * root1 = new headNode;
    root1 = hold;
    root1->code = stringArr;
    if(root1 == NULL)
    {
    }
    else if(root1->left_Child == NULL && root1->right_Child == NULL)
    {
        letters[orderOfLetters] = root1->content;
        codes[orderOfLetters] = root1->code;
        orderOfLetters++;
    }
    else
    {
        root1->left_Child->code = stringArr.append("1");
        stringArr.erase(stringArr.end()-1);
        root1->right_Child->code = stringArr.append("0");
        stringArr.erase(stringArr.end()-1);
        binaryBuilder(root1->left_Child,stringArr.append("1"));
        stringArr.erase(stringArr.end()-1);
        binaryBuilder(root1->right_Child,stringArr.append("0"));
        stringArr.erase(stringArr.end()-1);
    }
}

void sortByLetter()
{
    int i,j;
    string holder;
    for(i=0;i<orderOfLetters-1;i++)
    {
        for(j=0;j<orderOfLetters-i-1;j++)
        {
            if(letters[j]>letters[j+1])
            {
                holder=letters[j];
                letters[j]=letters[j+1];
                letters[j+1]=holder;
                holder=codes[j];
                codes[j]=codes[j+1];
                codes[j+1]=holder;
            }
        }
    }
}

void huffBinCode()
{
    int i;
    float freqHolder;
    string letterHolder = "";

    ifstream myFile;
    myFile.open("a4-1.txt");

    string stringOfInt;
    getline(myFile, stringOfInt);
    int intOfString = 0;

    for (i = 0; i < stringOfInt.size(); i++)
    {
        intOfString += (stringOfInt[i] - 48) * pow(10, (stringOfInt.size() - i - 1));
    }

    cout<<"Frequencies are:"<<endl;
    for(i = 0;i<intOfString;i++)
    {
        headNode holdingNode;
        myFile >> letterHolder >> freqHolder;
        cout << letterHolder << " " << freqHolder<<endl;

        holdingNode.content = letterHolder;
        holdingNode.freq = freqHolder;
        holdingNode.left_Child = 0;
        holdingNode.right_Child = 0;
        treeArr.push_back(holdingNode);
    }

    cout<<endl<<endl;

    headNode build = buildHTree();
    binaryBuilder(&build,"");
    sortByLetter();

    cout<<"Huffman Codes are:"<<endl;

    for(i=0;i<orderOfLetters;i++)
    {
        cout<<letters[i]<<" "<<codes[i]<<endl;
    }

    myFile.close();
}

int main()
{
    huffBinCode();
    string stringHolder;
    char chars[52];
    strcpy(chars,stringHolder.c_str());

    ifstream file2;
    file2.open("a4-2.txt");
    string binaryLine;
    getline(file2, binaryLine);

    string decodedString;
    int i;

    while(binaryLine.length()>0)
    {

        for(i=0;i<orderOfLetters;i++)
        {
            int n = codes[i].length();

            if(binaryLine.substr(0,n)==codes[i])
            {
                decodedString = decodedString + letters[i];
                binaryLine.erase (0,n);
                break;
            }
        }
    }

    cout<<endl<<"Decoded Binary Phrase: "<<decodedString<<endl;
    return 0;
}
