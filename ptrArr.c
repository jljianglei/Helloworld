#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef int (*arr)[6];
int main() {
    int a[6] = {0,1,2,3,4,5};
    printf("a %p,&a%p\n",a,&a);
    arr a1 = &a;
    printf("a1 %p,*a1 %p\n",a1,*a1);
    printf("%d %d\n",(*a1)[0],(*a1)[1]);
    printf("%d %d\n",(*a1)[2],(*a1)[3]);
    printf("%d %d\n",(*a1)[4],(*a1)[5]);
    return 0;
}
