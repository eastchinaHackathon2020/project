#include <iostream>
#include <string.h>
using namespace std;
#define mc memcpy
int main()
{
    char a[10000];
    unsigned char b[10000];
    for(int i=0;i<10000;i++)
        a[i]='0';
    mc(b,a,10000*sizeof(char));
    return 0;
}