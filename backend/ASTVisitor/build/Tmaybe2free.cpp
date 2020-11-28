#include <iostream>
int main()
{
    int a;
    std::cin>>a;
    int* p1=new int(7654321);
    int* p2=p1;
    int* p3=new int(444);
    int* p4=&a;

    delete p1;
    if(a>10000)
        p3=p1;
    delete p3;
    return 0;
}