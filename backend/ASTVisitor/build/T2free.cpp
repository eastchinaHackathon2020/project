#include <iostream>
int main()
{
    int a=7654321;
    int* p1=new int(7654321);
    int* p2=p1;
    int* p3;
    int* p4=&a;

    delete p1;
    delete p1;
    delete p2;
    delete p3;
    return 0;
}