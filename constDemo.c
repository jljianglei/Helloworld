#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
int main() {
    const int *a = NULL;
    int b = 10;
    a = &b;
    *a = 15;/* error,a pointer const b,can not modify b*/
    int * const p = NULL;
    p = &b;/* error,p is const pointer,can not modify p */
    const int * const c = NULL;
    c = &b;/* error,can not modify c*/
    *c = 20;/*error,can not modify *c*/
    return 0;
}
