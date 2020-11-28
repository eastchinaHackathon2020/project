#include <stdio.h>
#include <string.h>
#include<fstream>
#include<iostream>

using namespace std;
int inc(int a)
{
    return a++;
}

int main()
{
    int a =0;
    for(int i=0;i<10;i++)
    {
        a = inc(a);
    }
    if(a>1)
    {
        printf("maior\n");
    }
    else
    {
        printf("menor\n");
    }
ofstream OutFile("Test.txt");//利用构造函数创建txt文本，并且打开该文本

 OutFile << "This is a Test12!";//把字符串内容"This is a Test!"，写入Test.txt文件

 OutFile.close();


    return 0;
}
