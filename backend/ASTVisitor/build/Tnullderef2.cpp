#include<iostream>
using namespace std;
int main()
{
    int m=7654321;
    int* p1=nullptr;
    int* p2=&m;
    int am=*p2;
    p2=p1;
    int a=*p1;
    int b=*p2;
    return 0;
}