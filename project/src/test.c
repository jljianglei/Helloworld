#include<stdio.h>
#include<stdlib.h>
#include "add.h"
#include "sub.h"
int main() {
    int a = 10,b = 20;
    printf("%d + %d = %d\n",a,b,add(a,b));
    printf("%d - %d = %d\n",a,b,sub(a,b));
    return 0;
}
