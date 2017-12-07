#include<stdio.h>
#include<stdlib.h>
int main() {
   char* p = malloc(10);
   if(p == NULL) {
      printf("malloc faild\n");
	  goto FAILD;
   }
   else {
      printf("p = %p\n",p);
   }
   char * q;
   q = realloc(p,100);
   if(q == NULL) {
      printf("realloc faild\n");
	  goto FAILD;
   }
   else {
      printf("q = %p\n",q);
   }
   if(q == p) {
	  printf("free p\n");
      free(p);
   }
   else {
	  printf("free q\n");
      free(q);
   }
FAILD:
   return 0;
}
