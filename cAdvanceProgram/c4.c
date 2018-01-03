#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main() {
    int a[] = {1,2,3,4,5};
    printf("&a = %p\n",&a);
    int (*p)[] = &a;
    printf("p = %p\n",p);
    printf("%d\n",*(*p));
    return 0;
}
