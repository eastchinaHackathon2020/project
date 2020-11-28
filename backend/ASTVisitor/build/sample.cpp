#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE (1024*1024)
#define LARGE_MEMORY_LEN 4096
#define LARGE_MEMORY_LEN1 5120
#define LARGE_MEMORY_LEN2 2048

void my_memcpy(void* dest, void* src, unsigned int len){
    // 低速逐 byte 复制
    unsigned char* c_dest = (unsigned char*) dest;
    unsigned char* c_src  = (unsigned char*) src;
    for(int i = 0; i < SIZE; ++i)
        *c_dest++ = *c_src++;
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

int seexp1(){
    int ret=0;
    char seexp_str[5120];
    for (ret = 0; ret < 5120; ret++) {
        seexp_str[ret] = 'a';
    }
}

int seexp2(){
    char *dest_seexp = (char *)malloc(LARGE_MEMORY_LEN);
    int *p_seexp   = (int *)malloc(LARGE_MEMORY_LEN);
    memset(dest_seexp, 0x10, LARGE_MEMORY_LEN2);
    memset(p_seexp, 10, LARGE_MEMORY_LEN2);
    char *src_seexp = (char*)p_seexp;
    // seexp, warning at follow loop:
    for(int i = 0; i < LARGE_MEMORY_LEN2; i++) {
        if (*dest_seexp > *src_seexp) {
            *dest_seexp = *src_seexp;
        }
        else {
            *src_seexp = *dest_seexp;
        }
    }
    free(dest_seexp);
    free(p_seexp);
}

int seexp3(){
    char *dest_seexp = (char *)malloc(LARGE_MEMORY_LEN);
    char *src_seexp  = (char *)malloc(LARGE_MEMORY_LEN);
    memset(src_seexp, 0xf0, LARGE_MEMORY_LEN1);
    // seexp, warning at follow loop:
    int i = 0;
    while (i++ < LARGE_MEMORY_LEN1) {
        dest_seexp[i] = src_seexp[i];
    }
    free(dest_seexp);
    free(src_seexp);
}

int seexp4(){
    int seexp_len = 2048, seexp_i = 0;
    unsigned char* seexp_big_buf = (unsigned char*)malloc(seexp_i);
    while (seexp_i < seexp_len) {
        seexp_big_buf[seexp_i++] = 0xff; // seexp, warning
    }
    free(seexp_big_buf);
}

int seexp5(){
    char dest_seexp[LARGE_MEMORY_LEN2];
    int i = 0;
    // seexp, warning at follow loop:
    do {
        dest_seexp[i] = 'n';
    } while (++i < LARGE_MEMORY_LEN2);
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
    seexp1();
    seexp2();
    seexp3();
    seexp4();
    seexp5();

    return 0;
}
