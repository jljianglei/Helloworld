#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main() {
    int a,b,c;
    a = b = c = 1;
    int * p[] = {&a,&b,&c};
    printf("sizeof(p) = %d\n",sizeof(p));
    int i;
    for(i = 0; i  < sizeof(p)/sizeof(int*); ++i) {
        printf("%d ",*(*(p + i)));
    }
    printf("\n");
    return 0;
}
