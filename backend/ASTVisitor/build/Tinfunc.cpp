#include <iostream>
#define NULL 0
int nullp(int* p)
{
    p=NULL;
    int i=*p;
    return i;
}
int main()
{
    int* p=new int(8);
    int recv=nullp(p);

    return 0;
}