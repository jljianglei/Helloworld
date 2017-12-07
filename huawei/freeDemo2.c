#include<stdio.h>
#include<stdlib.h>
int main() {
   char * p = malloc(10);
   printf("p = %p\n",p);
   free(p);
   memcpy(p,"hello",5);
   printf("p = %p\n",p);
   char * q = malloc(10);
   printf("q = %p\n",q);
   printf("%s\n",q);
   return 0; 
}
