#include<stdlib.h>
#include<stdio.h>
#include<string.h>
void getMemory(int ** p) {
   *p = malloc(sizeof(int) * 5);
   if(p == NULL) {
      printf("malloc faild\n");
   }
}
int main() {
    int * q = NULL;
    getMemory(&q);
    printf("q = %p\n",q);
    *q = 12;
    if(q) {
       free(q);
       q = NULL;
    }
    return 0;
}
