#include <stdio.h>
#include <stdlib.h>
#define SIZE (1024*1024)

void my_memcpy(void* dest, void* src, unsigned int len){
    // 低速逐 byte 复制
    unsigned char* c_dest = (unsigned char*) dest;
    unsigned char* c_src  = (unsigned char*) src;
    for(int i = 0; i < len; ++i)
        *c_dest++ = *c_src++;
}

int my_memcmp(void* a, void* b, unsigned int len){
    // 低速逐 byte 比较
    unsigned char* c_a = (unsigned char*) a;
    unsigned char* c_b = (unsigned char*) b;
    for(int i = 0; i < len; ++i){
        unsigned char a = *c_a++;
        unsigned char b = *c_b++;
        if(a>b) return 1;
        if(b>a) return -1;
    }
    return 0;
}

int my_memcmp_short(void* a, void* b, unsigned int len){
    // 逐 short 比较
    short* c_a = (short*) a;
    short* c_b = (short*) b;
    for(int i = 0; i < len; ++i){
        short a = *c_a++, b = *c_b++;
        if(a>b) return 1;
        if(b>a) return -1;
    }
    return 0;
}

int my_memcmp_int(void* a, void* b, unsigned int len){
    // 逐 int 比较
    int* c_a = (int*) a;
    int* c_b = (int*) b;
    for(int i = 0; i < len; ++i){
        int a = *c_a++, b = *c_b++;
        if(a>b) return 1;
        if(b>a) return -1;
    }
    return 0;
}

int main(){
    unsigned char* a = NULL, *b = NULL;
    a = (unsigned char*)malloc(SIZE*sizeof(unsigned char));
    b = (unsigned char*)malloc(SIZE*sizeof(unsigned char));
    // do something for a and b
    // 对于较大的内存区域，采取低俗的逐 byte 比较或者复制操作
    printf("%d\n",my_memcmp(a, b, SIZE));
    my_memcpy(a, b, SIZE);
    printf("%d\n",my_memcmp(a, b, SIZE));
    return 0;
}
