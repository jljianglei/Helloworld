#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
int main() {
    int a = 10;
    printf("&a = %p\n",&a);
    int * p = &a;
    printf("&p = %p\n",&p);
    int ** q = &p;
    printf("&q = %p\n",&q);
    int *** k = &q;
    printf("&k = %p\n",&k);
    return 0;
}
