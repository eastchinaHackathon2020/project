#include <stdio.h>
#include <string.h>

int inc(int a)
{
    return a++;
}

int my_memcmp(void* a, void* b, unsigned int len){
    // 低速逐 byte 比较
    unsigned char* c_a = (unsigned char*) a;
    unsigned char* c_b = (unsigned char*) b;
    for(int i = 0; i < len; ++i){
        unsigned char a = *c_a++, b = *c_b++;
        if(a>b) return 1;
        if(b>a) return -1;
    }
    return 0;
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
    return 0;
}