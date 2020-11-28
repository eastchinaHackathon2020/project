#include <iostream>
#define NULL 0
void pfree(int *p)
{
    delete p;
}
void pnull(int *p,int* q)
{
    q=NULL;
    int r=*q;
}
int main()
{
    int* p1=new int(9);
    pfree(p1);
    delete p1;
    return 0;
}