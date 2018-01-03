#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main() {
    int * p = malloc(sizeof(int));
    if(p == NULL) {
       printf("p malloc faild\n");
       return -1;
    }
    *p = 10;
    printf("*p = %d\n",*p);
    int * newP = realloc(p,sizeof(int) * 10);
    if(newP == NULL) {
       printf("realloc faild\n");
       if(p) {
          free(p);
          p = NULL;
       }
       return -1;
    }
    if(newP == p) {
       printf("realloc addr is == malloc,need to free p\n");
       if(p) {
          free(p);
          p = NULL;
       }
       return 0;
    }
    printf("realloc addr is != malloc,p already free,just need free newP\n");
    if(newP) {
       free(newP);
       newP = NULL;
    }
    return 0;
}
